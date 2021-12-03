// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/29
// Description: 定义了一些socket相关操作
#ifndef SUB_MUDUO_SOCKETSOPS_H
#define SUB_MUDUO_SOCKETSOPS_H

#include <netinet/in.h>

namespace sub_muduo {
    namespace net {
        namespace sockets {
            //将sockaddr_in转换成sockaddr的转换函数的声明，放在sockets命名空间中，会用在bind和connect的时候
            //TODO:muduo库传入的是sockaddr_in6，然后再使用implicit_cast将其转换成sockaddr_in
            inline const struct sockaddr *sockaddr_cast(const struct sockaddr_in* addr) {
                return reinterpret_cast<const struct sockaddr *>(addr);
            }
        }
    }
}


#endif //SUB_MUDUO_SOCKETSOPS_H
