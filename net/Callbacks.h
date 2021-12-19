// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/18
// Description: 
#ifndef SUB_MUDUO_CALLBACKS_H
#define SUB_MUDUO_CALLBACKS_H

#include <functional>
#include <memory>

namespace sub_muduo {
    class Timestamp;
    namespace net {
        class Buffer;
        class TcpConnection;

        using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
        using TimerCallback = std::function<void()>;
        using ConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
        using CloseCallback = std::function<void(const TcpConnectionPtr &)>;
        using WriteCompleteCallback = std::function<void(const TcpConnectionPtr &)>;
        //TODO:HighWaterMarkCallback
        using MessageCallback = std::function<void(const ConnectionCallback &, Buffer *, Timestamp)>;
    }
}

#endif //SUB_MUDUO_CALLBACKS_H
