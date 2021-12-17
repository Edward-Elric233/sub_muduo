// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/2
// Description: 
#include "CountDownLatch.h"
#include "Condition.h"

namespace sub_muduo {

    CountDownLatch::CountDownLatch(int count)
    : count_(count)
    , condition_(mutex_) {}//使用互斥锁初始化条件变量

    void CountDownLatch::wait() {
        MutexLockGuard lockGuard(mutex_);   //查看count的状态也需要上锁
        while (count_ > 0) {    //必须使用while而不是if，虽然在这里影响不大
            condition_.wait();
        }
    }

    void CountDownLatch::countDown() {
        MutexLockGuard lockGuard(mutex_);//进入临界区，改变条件
        //因为只需要等待count变为0，所以broadcast一次就可以了，即使有多个wait也会被通知到
        if (--count_ == 0) condition_.notifyAll();  //signal/broadcast在不在临界区影响不大
    }

    int CountDownLatch::getCount() const {
        MutexLockGuard lockGuard(mutex_);//查看count的值需要上锁
        return count_;
    }

}