// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/18
// Description: 
#ifndef SUB_MUDUO_ACCEPTOR_H
#define SUB_MUDUO_ACCEPTOR_H

#include "noncopyable.h"
#include "Socket.h"
#include "Channel.h"

#include <functional>

namespace sub_muduo {
    namespace net {
        class EventLoop;
        class InetAddress;

        class Acceptor :noncopyable {
        public:
            using NewConnectionCallback = std::function<void(int sockfd, const InetAddress&)>;
            Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport);
            ~Acceptor();

            void setNewConnetionCallback(const NewConnectionCallback &cb) {
                //由TcpServer进行设置，用来处理新连接，必须进行设置
                //将新连接分发给subReactor
                newConnectionCallback_ = cb;
            }
            bool listenning() const {
                return listenning_;
            }
            void listen();
        private:
            void handleRead();
            EventLoop *loop_;   //Acceptor使用的就是mainLoop
            Socket acceptSocket_;
            Channel acceptChannel_;
            NewConnectionCallback newConnectionCallback_;
            bool listenning_;
        };

    }
}


#endif //SUB_MUDUO_ACCEPTOR_H
