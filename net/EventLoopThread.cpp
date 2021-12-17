// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/17
// Description: 
#include "EventLoopThread.h"
#include "EventLoop.h"

namespace sub_muduo {
    namespace net {

        EventLoopThread::EventLoopThread(const ThreadInitCallback &cb, const std::string &name)
            : loop_(nullptr)
            , exiting_(false)
            , thread_(std::bind(&EventLoopThread::threadFunc, this), name)
            , mutex_()
            , cond_(mutex_)
            , callback_(cb) {

        }

        EventLoopThread::~EventLoopThread() {
            exiting_ = true;
            if (loop_ != nullptr) {
                loop_->quit();
                thread_.join();
            }
        }

        EventLoop *EventLoopThread::startLoop() {
            thread_.start();    //启动线程，执行threadFunc

            EventLoop *loop = nullptr;
            {
                MutexLockGuard lock(mutex_);
                while (loop_ == nullptr) {
                    cond_.wait();
                }
                loop = loop_;
            }
            return loop;
        }

        void EventLoopThread::threadFunc() {
            EventLoop loop;     //创建EventLoop
            if (callback_) {
                callback_(&loop);
            }
            {
                MutexLockGuard lock(mutex_);
                loop_ = &loop;
                cond_.notify();
            }
            loop.loop();
            MutexLockGuard lock(mutex_);
            loop_ = nullptr;
        }

    }
}