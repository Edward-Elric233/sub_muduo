// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/11
// Description: 
#include "TcpServer.h"
#include "EventLoop.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Acceptor.h"
#include "EventLoopThreadPool.h"

namespace sub_muduo {
namespace net {

using std::placeholders::_1;
using std::placeholders::_2;

static EventLoop *CheckLoopNotNull(EventLoop *loop) {
    if (loop == nullptr) {
        //TODO:出错处理
    }
    return loop;
}

TcpServer::TcpServer(EventLoop *loop, const InetAddress &listenAddr, const string &nameArg, Option option)
    : loop_(CheckLoopNotNull(loop))
    , ipPort_(listenAddr.getIpPort())
    , name_(nameArg)
    , acceptor_(new Acceptor(loop, listenAddr, option == kReusePort))
    , threadPoll_(new EventLoopThreadPool(loop, nameArg))
    , connectionCallback_()
    , messageCallback_()
    , nextConnId_(1) {
    //注册aceept后运行的函数
    acceptor_->setNewConnetionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}

void TcpServer::newConnection(int sockfd, const InetAddress &peerAddr) {
    //选择一个sub Reactor，并将使用TcpConnection将生成的Channel绑定在对应的Eventloop上，然后注册回调
    EventLoop *ioLoop = threadPoll_->getNextLoop();

}

void TcpServer::setThreadNum(int numThreads) {
    //设置SubEventLoop的个数
    threadPoll_->setThreadNum(numThreads);
}

void TcpServer::start() {
    //防止被启动多次
    if (started_++ == 0) {
        threadPoll_->start(threadInitCallback_);    //启动多个Sub EventLoop
        //将连接描述符放在baseLoop的监听队列上
        loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));    //同acceptor_->listen
    }
}

}
}