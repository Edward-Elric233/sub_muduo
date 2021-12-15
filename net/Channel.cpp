// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/11
// Description: 
#include "Channel.h"
#include "EventLoop.h"
#include "Timestamp.h"
#include "Logger.h"

#include <poll.h>

using namespace sub_muduo;
using namespace sub_muduo::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;   //epoll和poll的事件是通用的，见https://stackoverflow.com/questions/30685515/are-poll-and-epoll-event-flags-compatible
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fd)
        : loop_(loop)
        , fd_(fd)
        , events_(0)
        , revents_(0)
        , index_(-1)
        , tied_(false) {}

Channel::~Channel() {
    //TODO:muduo库进行了一些assert操作
}

void Channel::tie(const std::shared_ptr<void> &obj) {
    //TODO:虽然还不清楚这个函数是干什么的
    tie_ = obj; //weak_ptr 绑定到 shared_ptr
    tied_ = true;
}

void Channel::update() {
    //在Poller中更新当前Channel所监听的事件，进行epoll_ctl操作
    //TODO:EventLoop中的updateChannel还没有写
    loop_->updateChannel(this);
}

void Channel::remove() {
    //TODO:同上没有实现
    loop_->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime) {
    std::shared_ptr<void> guard;
    if (tied_) {
        //如果绑定过需要尝试提升tie_，如果提升失败就算了
        guard = tie_.lock();
        if (guard) {
            handleEventWithGuard(receiveTime);
        }
    } else {
        handleEventWithGuard(receiveTime);
    }
}

//根据注册的事件调用回调
void Channel::handleEventWithGuard(Timestamp receiveTime) {

    LOG_INFO("channel handleEvent revents" + std::to_string(revents_));

    if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) { //出错了
        //POLLHUP 文件描述符挂起，即连接关闭（双向）
        //TODO:muduo库会向日志写入错误
        if (closeCallback_) closeCallback_();
    }
    if (revents_ & (POLLNVAL | POLLERR)) {
        //POLLNVAL 文件描述符非法
        //POLLERR 有错误发生
        if (errorCallback_) errorCallback_();
    }
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        //POLLIN 有数据可读
        //POLLPRI 有紧急数据可读
        //POLLRDHUP 连接半关闭：对方关闭写端或本地关闭读端
        if (readCallback_) readCallback_(receiveTime);
    }
    if (revents_ & POLLOUT) {
        //POLLOUT 有普通数据可写
        if (writeCallback_) writeCallback_();
    }
}