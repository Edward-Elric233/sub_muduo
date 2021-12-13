// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/11
// Description: 抽象层，使用poll/epoll进行IO复用
#ifndef SUB_MUDUO_POLLER_H
#define SUB_MUDUO_POLLER_H

#include "noncopyable.h"

#include <unordered_map>
#include <vector>

namespace sub_muduo {
    namespace net {

        class Channel;
        class EventLoop;
        class Timestamp;

        class Poller : noncopyable {
        public:
            using ChannelList = std::vector<Channel*>;

            Poller(EventLoop *loop);
            virtual ~Poller();  //虚基类必须使用虚析构函数，因为基类指向派生类的指针在析构的时候如果没有虚析构函数无法调用到派生类的析构函数导致内存泄露

            //阻塞监听事件，使用IO复用函数：纯虚函数，要求派生类重写
            virtual Timestamp poll(int timeoutMs, ChannelList &activeChannels) = 0;

            //更新channel的注册事件
            virtual void updateChannel(Channel *channel) = 0;
            //取消对channel对应文件描述符的监听
            virtual void removeChannel(Channel *channel) = 0;
            //判断channel是否由Poller监听
            virtual bool hasChannel(Channel *channel) const;
            //获取默认的Poller，默认应该是epoll实现，在poller/DefaultPoller.cpp中实现
            //之所以不在Poller.cpp中实现是因为Poller作为抽象层，应该尽可能将实现细节隐藏起来，放在同一个位置（poller文件夹）
            static Poller *newDefaultPoller(EventLoop *loop);

        protected:
            //TODO:muduo库使用map，没有什么必要，使用unordered_map效率更高
            using ChannelMap = std::unordered_map<int, Channel*>;
            ChannelMap channels_;   //保存文件描述符到Channel的映射
        private:
            EventLoop *ownerLoop_;  //Poller对应的事件循环，两者是一一对应
        };

    }
}


#endif //SUB_MUDUO_POLLER_H
