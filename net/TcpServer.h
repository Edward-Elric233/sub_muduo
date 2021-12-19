// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/11
// Description: 服务器编程使用的类
#ifndef SUB_MUDUO_TCPSERVER_H
#define SUB_MUDUO_TCPSERVER_H

#include "noncopyable.h"
#include "Types.h"
#include "Callbacks.h"

#include <functional>
#include <memory>
#include <atomic>
#include <map>

namespace sub_muduo {
    namespace net {
        class EventLoop;
        class EventLoopThreadPool;
        class Acceptor;
        class InetAddress;

        class TcpServer :noncopyable {
        public:
            //Reactor线程刚开始会运行这个函数，虽然还不知道要干什么
            using ThreadInitCallback = std::function<void(EventLoop *)>;
            enum Option{
                kNoReusePort,   //是否开启端口复用
                kReusePort,
            };
            TcpServer(EventLoop *loop, const InetAddress &listenAddr, const string &nameArg,
                      Option option = kNoReusePort);
            ~TcpServer();
            void setThreadInitCallback(const ThreadInitCallback &cb) {
                threadInitCallback_ = cb;
            }
            //设置subReactor的个数
            void setThreadNum(int numThreads);
            //开启服务器监听，acceptor.listen()
            void start();

            void setConnectionCallback(const ConnectionCallback &cb) {
                connectionCallback_ = cb;
            }
            void setMessageCallback(const MessageCallback &cb) {
                messageCallback_ = cb;
            }
            void setWriteCompleteCallback(const WriteCompleteCallback &cb) {
                writeCompleteCallback_ = cb;
            }
        private:
            void newConnection(int  sockfd, const InetAddress &peerAddr);
            void removeConnection(const TcpConnectionPtr &conn);
            void removeConnectionInLoop(const TcpConnectionPtr &conn);
            using ConnectionMap = std::map<string, TcpConnectionPtr>;
            EventLoop *loop_;       //baseLoop
            const string ipPort_;
            const string name_;
            std::unique_ptr<Acceptor> acceptor_;    //主Rector监听连接，并处理新连接
            std::shared_ptr<EventLoopThreadPool> threadPoll_;   //one loop per thread
            ConnectionCallback connectionCallback_; //新连接时回调
            MessageCallback messageCallback_;       //可读事件回调
            WriteCompleteCallback writeCompleteCallback_;   //写完后的回调
            ThreadInitCallback threadInitCallback_; //工作线程初始化的回调
            std::atomic_int32_t started_;
            int nextConnId_;
            ConnectionMap connections_; //保存所有连接
        };

    }
}


#endif //SUB_MUDUO_TCPSERVER_H
