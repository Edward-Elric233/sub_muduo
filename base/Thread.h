// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/15
// Description: 
#ifndef SUB_MUDUO_THREAD_H
#define SUB_MUDUO_THREAD_H

#include "noncopyable.h"
#include "CountDownLatch.h"
#include "Types.h"

#include <atomic>
#include <thread>
#include <functional>
#include <memory>

namespace sub_muduo {

    class Thread : noncopyable {
    public:
        using ThreadFunc = std::function<void()>;
        explicit Thread(ThreadFunc, const string &name = std::string());
        ~Thread();
        void start();
        int join();

        bool started() const {
            return started_;
        }
        pid_t tid() const {
            return tid_;
        }
        const string &name() const {
            return name_;
        }
        static int numCreated() {
            return numCreated_;
        }
    private:
        void setDefaultName();

        bool started_;
        bool joined_;
        std::shared_ptr<std::thread> thread_;
        pid_t tid_;
        ThreadFunc func_;
        string name_;
        CountDownLatch latch_;

        static std::atomic_int32_t numCreated_;
    };

}


#endif //SUB_MUDUO_THREAD_H
