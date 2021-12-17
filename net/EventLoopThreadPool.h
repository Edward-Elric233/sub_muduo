// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/17
// Description: 
#ifndef SUB_MUDUO_EVENTLOOPTHREADPOOL_H
#define SUB_MUDUO_EVENTLOOPTHREADPOOL_H

#include "noncopyable.h"
#include "Types.h"

#include <functional>
#include <string>
#include <vector>
#include <memory>

namespace sub_muduo {
    namespace net {

        class EventLoop;
        class EventLoopThread;

        class EventLoopThreadPool : noncopyable {
        public:
            using ThreadInitCallback = std::function<void(EventLoop*)>;

            EventLoopThreadPool(EventLoop *baseLoop, const std::string &nameArg);
            ~EventLoopThreadPool();
            //设置线程数量
            void setThreadNum(int numThreads) {
                numThreads_ = numThreads;
            }
            void start(const ThreadInitCallback &cb = ThreadInitCallback());
            //使用Round_Robin算法分发连接
            EventLoop* getNextLoop();
            //TODO:getLoopForHash
            std::vector<EventLoop*> getAllLoops();

            bool started() const {
                return started_;
            }

            const string &name() const {
                return name_;
            }

        private:
            EventLoop *baseLoop_;   //用户创建的loop，主reactor
            std::string name_;
            bool started_;
            int numThreads_;
            int next_;
            std::vector<std::unique_ptr<EventLoopThread>> threads_;
            std::vector<EventLoop *> loops_;
        };

    }
}


#endif //SUB_MUDUO_EVENTLOOPTHREADPOOL_H
