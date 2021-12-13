// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/28
// Description: 
#include "InetAddress.h"
#include "Types.h"

#include <arpa/inet.h>

namespace sub_muduo {

    namespace net {

        InetAddress::InetAddress(in_port_t port, bool loopbackOnly, bool ipv6) {
            if (!ipv6) {
                //ipv4
                ::bzero(&addr_, sizeof (addr_));
                addr_.sin_family = AF_INET;
                //如果是回环地址，则将ip设置为127.0.0.1，否则设置为0.0.0.0
                in_addr_t ip = loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY;
                //muduo库使用的是宏htobe32，实际上调用的是函数__bswap_32，这个函数会将翻转数字的字节序。但是我认为还是使用hton函数好一些
                //因为不一定本地就是小端模式
                addr_.sin_port = htons(port);
                addr_.sin_addr.s_addr = htonl(ip);
            }
        }

        InetAddress::InetAddress(const string &ip, in_port_t port, bool ipv6) {
            if (!ipv6) {
                //ipv4
                ::bzero(&addr_, sizeof(addr_));
                addr_.sin_family = AF_INET;
                addr_.sin_port = htons(port);
                ::inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr.s_addr);
            }
        }

        const string InetAddress::getIp() const {
            if (addr_.sin_family == AF_INET) {
                char buf[INET_ADDRSTRLEN];
                ::inet_ntop(AF_INET, &addr_.sin_addr.s_addr, buf, INET_ADDRSTRLEN);
                return buf;
            } else if (addr_.sin_family == AF_INET6) {
                //ipv6
                return "";
            }
        }

        in_port_t InetAddress::getPort() const {
            return ntohs(addr_.sin_port);
        }

        const string InetAddress::getIpPort() const {
            string ip = getIp();
            return ip.append(":").append(std::to_string(getPort()));
        }
    }

}