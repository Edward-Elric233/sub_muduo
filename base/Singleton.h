// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/3
// Description: 单例模式模板，使用pthread_once而不是DCL(双检查锁)实现多线程安全
#ifndef SUB_MUDUO_SINGLETON_H
#define SUB_MUDUO_SINGLETON_H

#include "noncopyable.h"

#include <pthread.h>

namespace sub_muduo {

    template<typename T>
    class SingleTon : noncopyable {
        static pthread_once_t once;
        static T *value;
    public:
        //构造和析构都删除掉，这个类更多相当于一个接口，只有静态数据和静态方法
        SingleTon() =delete;
        ~SingleTon() = delete;

        static T &instance() {
            pthread_once(&once, init_routine);
            return value;
        }

    private:
        static void init_routine(void) {
            value = new T;
            //TODO:muduo库还提供了删除的方法，目前觉得没必要
        }
    };

    //虽然按照Effective C++的建议，不要使用non-local static对象，但是这里只是供instance函数调用，无伤大雅
    template<typename T>
    pthread_once_t SingleTon<T>::once = PTHREAD_ONCE_INIT;

    template<typename T>
    T *SingleTon<T>::value = nullptr;

}

#endif //SUB_MUDUO_SINGLETON_H
