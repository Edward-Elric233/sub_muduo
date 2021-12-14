// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/14
// Description: 
#include "CurrentThread.h"

#include <unistd.h>
#include <sys/syscall.h>

namespace sub_muduo {
    namespace CurrentThread {
        __thread int t_cachedTid = 0;   //初始化为0

        void cacheTid() {
            if (t_cachedTid == 0) {
                //通过syscall系统调用获取当前线程的tid
                t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
            }
        }

    }
}
