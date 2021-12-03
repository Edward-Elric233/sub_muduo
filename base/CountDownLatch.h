// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/2
// Description: 倒计时器，使用条件变量等待count变成0
#ifndef SUB_MUDUO_COUNTDOWNLATCH_H
#define SUB_MUDUO_COUNTDOWNLATCH_H

#include "noncopyable.h"
#include "Condition.h"

namespace sub_muduo {

    class CountDownLatch : noncopyable {
        int count_; //需要等待的计数
        Condition condition_;   //条件变量
        mutable MutexLock mutex_;   //保护计数器，必须要是mutable的
    public:
        explicit CountDownLatch(int count);
        //等待计数器变为0
        void wait();
        //将计数器减一
        void countDown();

        int getCount() const;
    };
}


#endif //SUB_MUDUO_COUNTDOWNLATCH_H
