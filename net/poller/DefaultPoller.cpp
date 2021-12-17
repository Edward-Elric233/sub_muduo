// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/11
// Description: 实现Poller::newDefaultPoller()

#include "Poller.h"
#include "EPollPoller.h"

#include <stdlib.h>

namespace sub_muduo {
    namespace net {

        Poller *Poller::newDefaultPoller(EventLoop *loop) {
            if (::getenv("SUB_MUDUO_USE_POLL")) {
                //使用poll实现
            } else {
                //使用epoll实现
                return new EPollPoller(loop);
            }
            return nullptr;
        }
    }
}
