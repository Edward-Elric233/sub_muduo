// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/2
// Description: 条件变量
#ifndef SUB_MUDUO_CONDITION_H
#define SUB_MUDUO_CONDITION_H

#include "Mutex.h"
#include "noncopyable.h"

#include <pthread.h>

//TODO:添加出错处理和对线程的支持

namespace sub_muduo {

    //封装条件变量的操作，很底层，一般不直接使用，而是用其封装高级同步措施
    class Condition : noncopyable {
        MutexLock &mutex_;   //用于保护对条件的查看和修改
        pthread_cond_t cond_;
    public:
        explicit Condition(MutexLock &mutex): mutex_(mutex) {
            pthread_cond_init(&cond_, nullptr);
        }
        ~Condition() {
            pthread_cond_destroy(&cond_);
        }
        void wait() {
            pthread_cond_wait(&cond_, mutex_.getPthreadMutex());
        }
        void notify() {
            pthread_cond_signal(&cond_);
        }
        void notifyAll() {
            pthread_cond_broadcast(&cond_);
        }
    };

}

#endif //SUB_MUDUO_CONDITION_H
