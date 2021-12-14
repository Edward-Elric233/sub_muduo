// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/14
// Description: 
#ifndef SUB_MUDUO_CURRENTTHREAD_H
#define SUB_MUDUO_CURRENTTHREAD_H

#include "Types.h"

namespace sub_muduo {
    namespace CurrentThread {
        //加上__thread参数表示虽然是全局变量，但是每个线程之间互相独立
        extern __thread int t_cachedTid;

        void cacheTid();

        inline int tid() {
            //__builtin_expect是告诉gcc编译器哪个分支指令出现的概率更大，让编译器进行分支预取指令优化
            //这里只有第一次访访问的时候需要取cacheTid，后面都是直接返回缓存的tid，所以为false的概率更大
            if (__builtin_expect(t_cachedTid == 0, false)) {
                cacheTid();
            }
            return t_cachedTid;
        }
    }
}


#endif //SUB_MUDUO_CURRENTTHREAD_H
