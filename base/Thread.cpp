// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/15
// Description: 
#include "Thread.h"
#include "CurrentThread.h"

namespace sub_muduo {

std::atomic_int32 numCreated_ = 0;

Thread::Thread(ThreadFunc, const string &name)
    : started_(false)
    , joined_(false)
    , tid_(0)
    , func_(std::move(ThreadFunc))
    , name_(name)
    , latch_(1) {
    setDefaultName();
}

void Thread::setDefaultName() {
    int num = ++numCreated_;
    if (name_.empty()) {
        name_ = "Thread";
        name_.append(std::to_string(num));
    }
}

Thread::~Thread() {
    if (started_ && !joined_) {
        thread_->detach();      //thread类提供的设置分离线程的方法
    }
}

void Thread::start() {
    started_ = true;
    thread_ = std::make_shared<std::thread>([&]() {
        //子线程
        tid_ = CurrentThread::tid();
        latch_.countDown();
        func_();
    });
    //等待获取子线程id
    latch_.wait();
}

int Thread::join() {
    joined_ = true;
    thread_->join();
}

}