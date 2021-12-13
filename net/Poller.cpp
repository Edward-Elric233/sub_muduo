// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/11
// Description: 
#include "Poller.h"
#include "Channel.h"

using namespace sub_muduo;
using namespace sub_muduo::net;

Poller::Poller(EventLoop *loop):ownerLoop_(loop) {}
Poller::~Poller() = default;

bool Poller::hasChannel(Channel *channel) const {
    ChannelMap::const_iterator it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel;
}
