// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/12
// Description: 用epoll实现Poller
#ifndef SUB_MUDUO_EPOLLPOLLER_H
#define SUB_MUDUO_EPOLLPOLLER_H

#include "Poller.h"

#include <vector>
#include <sys/epoll.h>

namespace sub_muduo {
    namespace net {

        class EPollPoller : public Poller {
        public:
            EPollPoller(EventLoop *loop);
            ~EPollPoller() override;

            //epoll_wait
            Timestamp poll(int timeoutMs, ChannelList &activeChannels) override;
            //Channel::update(Channel *) -> loop_->updateChannel(Channel *channel) ->
            void updateChannel(Channel *channel) override;
            //epoll_ctl_del
            void removeChannel(Channel *channel) override;

        private:
            static constexpr int kInitEventListSize = 16;

            void fillActiveChannels(int numEvents, ChannelList &activeChannels) const;
            //epoll_ctl
            void update(int operatoin, Channel *channel);

            using EventList = std::vector<epoll_event>;

            int epollfd_;
            EventList events_;
        };

    }
}



#endif //SUB_MUDUO_EPOLLPOLLER_H
