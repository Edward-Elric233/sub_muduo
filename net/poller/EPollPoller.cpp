// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/12
// Description: 
#include "EPollPoller.h"
#include "Channel.h"
#include "Timestamp.h"
#include "Types.h"

#include <unistd.h>
#include <errno.h>

using namespace sub_muduo;
using namespace sub_muduo::net;

namespace {
    //使用匿名枚举类型，muduo库是分别定义的，含义相同
    //对应Channel::index_的状态
    enum {
        kNew = -1,
        kAdded = 1,
        kDeleted = 2
    };
}

EPollPoller::EPollPoller(EventLoop *loop)
        : Poller(loop)
        , epollfd_(::epoll_create1(EPOLL_CLOEXEC))  //fork后在子进程中自动关闭文件描述符
        , events_(kInitEventListSize) {
    if (epollfd_ < 0) {
        //TODO:错误检查 LOG_SYSFATAL
    }
}

EPollPoller::~EPollPoller() noexcept {
    ::close(epollfd_);
}

//更新channel的状态
void EPollPoller::updateChannel(Channel *channel) {
    const int index = channel->index(); //获取Channel的状态
    //TODO:Log输出
    int fd = channel->fd();
    if (index == kNew || index == kDeleted) {
        //没有被监听，（重新）进行监听
        if (index == kNew) {
            channels_[fd] = channel;
        } else {
            //如果已经deleted了，则channels里面还有fd对应的channel
        }
        channel->setIndex(kAdded);
        update(EPOLL_CTL_ADD, channel);
    } else {
        if (channel->isNonEvent()) {
            update(EPOLL_CTL_DEL, channel);
            channel->setIndex(kDeleted);
        } else {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

//将channel从Poller的channels_中删除
void EPollPoller::removeChannel(Channel *channel) {
    channels_.erase(channel->fd());
    //TODO:log
    if (channel->index() == kAdded) {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->setIndex(kNew);
}

//调用epoll_ctl函数更新
void EPollPoller::update(int operatoin, Channel *channel) {
    struct epoll_event event;
    ::memset(&event, 0, sizeof(event));
    event.events = channel->events();
    event.data.ptr = channel;
    //TODO:log
    int fd = channel->fd();
    //TODO:出错检查
    epoll_ctl(epollfd_, operatoin, fd, &event);
}

//epoll_wait函数
Timestamp EPollPoller::poll(int timeoutMs, ChannelList &activeChannels) {
    //muduo库在这里使用的是&*events_.begin()，感觉很必要，可能大佬忘记data函数了吧
    int numEvents = ::epoll_wait(epollfd_,
                                 events_.data(),
                                 static_cast<int>(events_.size()),
                                 timeoutMs);
    int savedErrno = errno; //保证errno在每个线程都不一样，可能是通过使用__thread修饰符
    //默认构造函数会获取当前时间
    Timestamp now;

    if (numEvents > 0) {
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents == events_.size()) {
            //如果发生的事件已经等于events_的大小了，说明可能有更多的事件发生，就对events_进行扩容
            events_.resize(events_.size() * 2);
        }
    } else if (numEvents == 0) {
        //TODO:log
    } else {
        //TODO:log 出错处理
        if (savedErrno != EINTR) {
            //不是中断引起的错误，而是其他错误，那么就退出了
            errno = savedErrno; //防止errno被其他地方修改，但是感觉不是很必要，在同一个线程里应该errno是线程安全的吧
            //TODO:log LOG_SYSERR
        }
    }
    return now;
}

void EPollPoller::fillActiveChannels(int numEvents, ChannelList &activeChannels) const {
    activeChannels.clear();
    for (int i = 0; i < numEvents; ++i) {
        Channel *channel = static_cast<Channel *>(events_[i].data.ptr);
        //设置Channel上发生的事件
        channel->setRevents(events_[i].events);
        //将其加入activeChannels中，返回给EventLoop
        activeChannels.push_back(channel);
    }
}