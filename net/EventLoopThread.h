// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/17
// Description: 
#ifndef SUB_MUDUO_EVENTLOOPTHREAD_H
#define SUB_MUDUO_EVENTLOOPTHREAD_H

#include "noncopyable.h"
#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"

#include <functional>
#include <string>

namespace sub_muduo {
    namespace net {

        class EventLoop;

        class EventLoopThread : noncopyable {
        public:
            using ThreadInitCallback = std::function<void(EventLoop *)>;
            EventLoopThread(const ThreadInitCallback &cb = ThreadInitCallback(), const std::string &name = std::string());
            ~EventLoopThread();

            EventLoop *startLoop();
        private:
            void threadFunc();

            EventLoop *loop_;
            bool exiting_;
            Thread thread_;
            mutable MutexLock mutex_;
            Condition cond_;    //必须声明在MutexLockd的后面，因为要使用mutex_进行初始化
            ThreadInitCallback callback_;   //在新线程开始的时候运行，新线程会将创建的EventLoop传递给函数
        };

    }
}


#endif //SUB_MUDUO_EVENTLOOPTHREAD_H
