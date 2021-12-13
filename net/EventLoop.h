// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/11
// Description: 管理事件循环的类
#ifndef SUB_MUDUO_EVENTLOOP_H
#define SUB_MUDUO_EVENTLOOP_H

#include "noncopyable.h"

namespace sub_muduo {

    class EventLoop : noncopyable {
    public:
        EventLoop();
        ~EventLoop();
        void loop();
    };
}


#endif //SUB_MUDUO_EVENTLOOP_H
