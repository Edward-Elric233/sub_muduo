// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/11
// Description: 
#include "EventLoop.h"
#include "Poller.h"
#include "Channel.h"

#include <sys/eventfd.h>
#include <unistd.h>
#include <fcntl.h>

namespace sub_muduo {
namespace net {

//防止一个线程创建多个EventLoop
__thread EventLoop *t_loopInThisThread = nullptr;

//定义默认poller.poll的超时时间
constexpr int kPollTimeMs = 10000;

//创建wakerupfd，用来唤醒subReactor处理新来的channel
int createEventfd() {
    int evtfd = ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    //TODO:log出错处理
    return evtfd;
}

EventLoop::EventLoop()
    : looping_(false)
    , quit_(false)
    , callingPendingFunctors_(false)
    , threadId_(CurrentThread::tid())
    , poller_(Poller::newDefaultPoller(this))
    , wakeupFd_(createEventfd())
    , wakeupChannel_(new Channel(this, wakeupFd_)) {
    //TODO:log
    t_loopInThisThread = this;
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}

//将subReacotor从Poller.poll唤醒
void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t n = ::read(wakeupFd_, &one, sizeof one);
    if (n != sizeof one) {
        //TODO:log_error
    }
}

EventLoop::~EventLoop() {
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();   //在wakeupChannel_->enableReading()的时候添加的
    ::close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void EventLoop::loop() {
    looping_ = true;
    quit_ = false;
    //TODO:log
    while (!quit_) {
        activeChannels_.clear();
        //muduo库传递的是指针，我传递的是引用
        pollReturnTime_ = poller_->poll(kPollTimeMs, activeChannels_);
        for (auto channel : activeChannels_) {
            channel->handleEvent(pollReturnTime_);
        }
        //执行subEventLoop事件循环需要处理的回调操作
        doPendingFunctors();
    }
    looping_ = false;
}

//退出事件循环
void EventLoop::quit() {
    quit_ = true;
    if (!isInLoopThread()) {
        wakeup();   //如果不在EventLoop的线程，则EventLoop可能还在阻塞，因此需要主动唤醒一下让他自己退出
    }
}

void EventLoop::runInLoop(Functor cb) {
    if (isInLoopThread()) {
        cb();
    } else {
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(Functor cb) {
    {
        MutexLockGuard lockGuard(mutex_);
        pendingFunctors_.emplace_back(std::move(cb));
    }
    if (!isInLoopThread() || callingPendingFunctors_) {
        //唤醒loop
        wakeup();
    }
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof one);
    if (n != sizeof one) {
        //TODO:log_error
    }
}

void EventLoop::doPendingFunctors() {
    decltype(pendingFunctors_) functors;
    callingPendingFunctors_ = true;
    {
        //尽量缩小临界区，防止添加回调被阻塞
        MutexLockGuard lockGuard(mutex_);
        functors.swap(pendingFunctors_);
    }
    for (auto &functor : functors) {
        functor();
    }
    callingPendingFunctors_ = false;
}

void EventLoop::updateChannel(Channel *channel) {
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel) {
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel) {
    return poller_->hasChannel(channel);
}

}
}