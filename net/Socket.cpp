// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/17
// Description: 
#include "Socket.h"
#include "SocketsOps.h"
#include "InetAddress.h"
#include "Types.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>

namespace sub_muduo {
namespace net {

Socket::~Socket() {
    sockets::close(sockfd_);
}

void Socket::bindAddress(const InetAddress &localaddr) {
    sockets::bindOrDie(sockfd_, localaddr.getSockAddr());
}

void Socket::listen() {
    sockets::listenOrDie(sockfd_);
}

int Socket::accept(InetAddress *peeraddr) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    int connfd = sockets::accept(sockfd_, &addr);
    peeraddr->setSockAddr(addr);
    return connfd;
}

void Socket::shutdownWrite() {
    sockets::shutdownWrite(sockfd_);
}

void Socket::setTcpNoDelay(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReusePort(bool on) {
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                           &optval, static_cast<socklen_t>(sizeof optval));
    if (ret < 0 && on)
    {
        //TODO:出错处理
    }
}

void Socket::setKeepAlive(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

}
}