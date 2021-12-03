// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/2
// Description: 声明MutexLock类和MutexLockGuard类，使用RAII(Resource Acquisition Is Initialization)的手段对互斥量进行封装
#ifndef SUB_MUDUO_MUTEX_H
#define SUB_MUDUO_MUTEX_H

#include "noncopyable.h"
#include <pthread.h>

//TODO:没有进行出错检查，日后补上

namespace sub_muduo {

    //封装pthread_mutex_t，多作为（mutable）成员变量，配合MutexLockGuard使用
    //TODO:还需要封装一些和线程相关的
    class MutexLock : noncopyable {
        pthread_mutex_t mutex_;
        pid_t holder_;
    public:
        MutexLock(): holder_(0) {
            pthread_mutex_init(&mutex_, nullptr);
        }
        ~MutexLock() {
            //好像还要检查holder_，但是我目前不知道holder是干什么的
            pthread_mutex_destroy(&mutex_);
        }

    private:
        //muduo库将这两个函数放在public中，然后在注释中说，不要直接用，应该配合MutexLockGuard进行使用，但是我觉得还是直接放在private里面，你想用也用不了
        void lock() {
            pthread_mutex_lock(&mutex_);
        }
        void unlock() {
            pthread_mutex_unlock(&mutex_);
        }
        pthread_mutex_t *getPthreadMutex() {//差点想声明成const的了，但是返回的不是一个const指针，这个函数是供条件变量使用的
            return &mutex_;
        }

        friend class Condition;     //条件变量是友元类，会调用lock、unlock、getPthreadMutex
        friend class MutexLockGuard;//MutexLockGuard类也是友元类，因为要进行lock和unlock操作
    };

    //通过RAII的手法封装对互斥量的上锁和解锁，多作为栈上对象，在临界区开始时创建，结束时销毁
    //必须要有变量名，否则会变成一个临时对象，锁了个寂寞，muduo库对这种情况进行了检查，我就不检查了
    class MutexLockGuard : noncopyable {
        MutexLock &mutex_;  //引用
    public:
        explicit MutexLockGuard(MutexLock &mutex): mutex_(mutex) {
            mutex_.lock();
        }
        ~MutexLockGuard() {
            mutex_.unlock();
        }
    };

}   //namespace sub_muduo

#endif //SUB_MUDUO_MUTEX_H
