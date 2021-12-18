// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/18
// Description: 
#include "Acceptor.h"
#include "SocketsOps.h"
#include "InetAddress.h"

namespace sub_muduo {
namespace net {

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport)
    : loop_(loop)
    , acceptSocket_(sockets::createNonblockingOrDie(PF_INET))
    , acceptChannel_(loop, acceptSocket_.fd())
    , listenning_(false) {
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(reuseport);
    acceptSocket_.bindAddress(listenAddr);  //bind
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() {
    acceptChannel_.disableAll();    //取消监听
    acceptChannel_.remove();
}

void Acceptor::handleRead() {
    InetAddress peerAddr;
    int connfd = acceptSocket_.accept(&peerAddr);   //accept
    if (connfd >= 0) {
        if (newConnectionCallback_) {
            newConnectionCallback_(connfd, peerAddr);
        } else {
            sockets::close(acceptSocket_.fd());
        }
    } else {
        //TODO:出错处理
        if (errno == EMFILE) {
            //文件描述符达到上限
        }
    }
}

void Acceptor::listen() {
    listenning_ = true;
    acceptSocket_.listen(); //listen
    acceptChannel_.enableReading(); //在Poller上注册监听
}

}
}