// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/2
// Description: 阻塞队列模板
// 模板的声明和定义要在同一个文件中，这样才能进行实例化
#ifndef SUB_MUDUO_BLOCKINGQUEUE_H
#define SUB_MUDUO_BLOCKINGQUEUE_H

#include "Condition.h"
#include "Mutex.h"
#include "noncopyable.h"

#include <deque>

namespace sub_muduo {

    //模板类，队列里面放什么都行
    template<typename T>
    class BlockingQueue : noncopyable {
    public:
        using queue_type = std::deque<T>;
    private:
        queue_type queue_;  //存放消息
        mutable MutexLock mutex_;   //必须声明在condition_的前面
        Condition condition_;
    public:
        BlockingQueue(): condition_(mutex_) {}

        void put(const T &x);

        void put(T&& x);

        T take();

        size_t size() const;
        //TODO:没有搞清除这个函数是干什么的
        queue_type drain();
    };

    template<typename T>
    void BlockingQueue<T>::put(const T &x) {
        MutexLockGuard lockGuard(mutex_);   //进入临界区
        queue_.push_back(x);
        condition_.notify();    //唤醒一个或多个线程起来拿走消息，这里不能只在queue变成非空的时候通知，因为那样如果同时push_back多个只通知一个线程起来，是不好的
    }

    template<typename T>
    void BlockingQueue<T>::put(T &&x) {
        MutexLockGuard lockGuard(mutex_);
        queue_.push_back(std::move(x));
        condition_.notify();
    }

    template<typename T>
    T BlockingQueue<T>::take() {
        MutexLockGuard lockGuard(mutex_);
        while (queue_.empty()) {
            condition_.wait();
        }
        T front(std::move(queue_.front()));
        queue_.pop_front();
        return front;
    }

    template<typename T>
    typename BlockingQueue<T>::queue_type BlockingQueue<T>::drain() {   //才知道前面要加typename

    }
}

#endif //SUB_MUDUO_BLOCKINGQUEUE_H
