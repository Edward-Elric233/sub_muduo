// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/28
// Description: 封装struct sockaddr相关操作，保存socket的ip:port信息
#ifndef SUB_MUDUO_INETADDRESS_H
#define SUB_MUDUO_INETADDRESS_H

#include "copyable.h"
#include "sockets_ops.h"
#include "Types.h"

#include <netinet/in.h>

namespace sub_muduo {

    namespace net {

        class InetAddress : copyable{
            union {
                struct sockaddr_in addr_;
                struct sockaddr_in6 addr6_; //TODO:muduo库是支持IPV6的，但是目前sub_muduo不支持
            };
        public:
            /*!
             * 用于server使用bind函数绑定端口
             * @param port 设定端口
             * @param loopbackOnly 是否设定为本地回环地址127.0.0.1
             * @param ipv6 是否是ipv6地址
             */
            explicit InetAddress(in_port_t port, bool loopbackOnly = false, bool ipv6 = false);

            //指定IP地址和端口号
            InetAddress(const string &ip, in_port_t port, bool ipv6 = false);

            //accpet一个连接，传入一个sockaddr_in
            explicit InetAddress(const struct sockaddr_in& addr): addr_(addr) {}

            sa_family_t getFamily() const {return addr_.sin_family;}
            const string getIp() const;
            in_port_t getPort() const;
            const string getIpPort() const;
            //返回sockaddr *，用于bind
            const struct sockaddr* getSockAddr() const {return sockets::sockaddr_cast(&addr_);}

        };

    }   //namespace net
}   //namespace sub_muduo


#endif //SUB_MUDUO_INETADDRESS_H
