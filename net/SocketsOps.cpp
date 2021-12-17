// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/29
// Description: 
#include "SocketsOps.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace sub_muduo {
namespace net {

void sockets::close(int fd) {
    if (::close(fd) < 0) {
        //TODO:错误处理
    }
}

void sockets::bindOrDie(int sockfd, const struct sockaddr *addr) {
    int ret = ::bind(sockfd, addr, sizeof (sockaddr_in));
    if (ret < 0) {
        //TODO:出错处理
    }
}

void sockets::listenOrDie(int sockfd) {
    //对应muduo库的SOMAXCONN
    int ret = ::listen(sockfd, 4096);
    if (ret < 0) {
        //TODO:出错处理
    }
}

int sockets::accept(int sockfd, struct sockaddr_in *addr) {
    socklen_t addrlen = sizeof(sockaddr_in);
    /*!
     * 在返回连接的scokfd的同时将其设置成非阻塞的，并且在子进程中自动关闭
     */
    //TODO:将转型封装起来
    int connfd = ::accept4(sockfd, reinterpret_cast<sockaddr *>(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd < 0) {
        //错误处理
    }
    return connfd;
}

void sockets::shutdownWrite(int sockfd) {
    if (::shutdown(sockfd, SHUT_WR) < 0) {
        //TODO:出错处理
    }
}

}
}
