// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/11
// Description: 管理事件循环的类
#ifndef SUB_MUDUO_EVENTLOOP_H
#define SUB_MUDUO_EVENTLOOP_H

#include "noncopyable.h"
#include "Timestamp.h"
#include "Mutex.h"
#include "CurrentThread.h"

#include <functional>
#include <vector>
#include <atomic>
#include <memory>

namespace sub_muduo {
    namespace net {
        class Channel;
        class Poller;

        class EventLoop : noncopyable {
        public:
            using Functor = std::function<void()>;

            EventLoop();
            ~EventLoop();

            //开启事件循环
            void loop();
            //退出事件循环
            void quit();

            Timestamp pollReturnTime() const {
                return pollReturnTime_;
            }

            //在当前loop中执行
            void runInLoop(Functor cb);
            //把cb放入执行队列中，唤醒loop所在的线程执行
            void queueInLoop(Functor cb);

            //唤醒loop所在线程
            void wakeup();
            //channel和poller通信，更新channel所监听的事件
            void updateChannel(Channel *channel);
            //channel和poller通信，删除channel
            void removeChannel(Channel *channel);
            bool hasChannel(Channel *channel);


            bool isInLoopThread() const {
                return threadId_ == CurrentThread::tid();
            }

        private:
            using ChannelList = std::vector<Channel *>;

            //wake up
            void handleRead();
            //执行回调
            void doPendingFunctors();

            std::atomic_bool looping_;  //原子操作，通过CAS实现，虽然我还不懂什么是CAS
            std::atomic_bool quit_;     //标志退出loop循环
            std::atomic_bool eventHandling_;
            const pid_t threadId_;  //记录创建Eventloop所在线程的id
            Timestamp pollReturnTime_;  //记录EPollPoller.poll的返回值：epoll_wait/poll/select返回的时间
            std::unique_ptr<Poller> poller_;
            //TODO:计时器
            //主要作用，当mainReactor获取一个新用户的Channel，通过轮询算法（Round_Robin）选择一个subReactor
            //通过该成员唤醒，通过eventfd函数
            int wakeupFd_;
            std::unique_ptr<Channel> wakeupChannel_;

            ChannelList activeChannels_;

            mutable MutexLock mutex_;                   //保护pendingFunctors_的线程安全

            std::atomic_bool callingPendingFunctors_;    //标识当前loop是否有需要执行的回调操作
            std::vector<Functor> pendingFunctors_;      //存储loop需要执行的所有回调操作
        };

    }
}


#endif //SUB_MUDUO_EVENTLOOP_H
