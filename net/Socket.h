// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/17
// Description: 
#ifndef SUB_MUDUO_SOCKET_H
#define SUB_MUDUO_SOCKET_H

#include "noncopyable.h"

namespace sub_muduo {
    namespace net {

        class InetAddress;
        //封装socketfd
        class Socket : noncopyable {
        public:
            explicit Socket(int sockfd): sockfd_(sockfd) {}
            ~Socket();

            int fd() const {
                return sockfd_;
            }
            void bindAddress(const InetAddress &localaddr);
            void listen();
            int accept(InetAddress *peeraddr);
            void shutdownWrite();

            void setTcpNoDelay(bool on);
            void setReuseAddr(bool on);
            void setReusePort(bool on);
            void setKeepAlive(bool on);
        private:
            const int sockfd_;
        };

    }
}


#endif //SUB_MUDUO_SOCKET_H
