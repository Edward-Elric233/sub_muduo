// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/11
// Description: 负责一个文件描述符的IO事件分发
#ifndef SUB_MUDUO_CHANNEL_H
#define SUB_MUDUO_CHANNEL_H

#include "noncopyable.h"

#include <functional>
#include <memory>

namespace sub_muduo {
    namespace net {

        class Timestamp;    //Timestamp类的前置声明
        class EventLoop;    //EventLoop类的前置声明


        class Channel : noncopyable {
        public:
            using EventCallback = std::function<void()>;
            using ReadEventCallback = std::function<void(Timestamp)>;

            Channel(EventLoop *loop, int fd);
            ~Channel();
            //得到Poller通知以后，根据发生的事件调用对应的回调
            void handleEvent(Timestamp receiveTime);
            //设置回调函数对象
            void setReadCallback(ReadEventCallback cb) {
                readCallback_ = std::move(cb);
            }
            void setWriteCallback(EventCallback cb) {
                writeCallback_ = std::move(cb);
            }
            void setCloseCallback(EventCallback cb) {
                closeCallback_ = std::move(cb);
            }
            void setErrorCallback(EventCallback cb) {
                errorCallback_ = std::move(cb);
            }
            //防止正在执行的回调Channel被手动remove掉
            void tie(const std::shared_ptr<void>&);

            int fd() const {
                return fd_;
            }

            int index() const {
                return index_;
            }
            void setIndex(int idx) {
                index_ = idx;
            }

            int events() const {
                return events_;
            }
            //供Poller调用
            int setRevents(int rvt) {
                revents_ = rvt;
            }
            bool isNonEvent() const {
                return events_ == kNoneEvent;   //判断是否没有注册事件
            }
            bool isReading() const {
                return events_ & kReadEvent;
            }
            bool isWriting() const {
                return events_ & kWriteEvent;
            }
            void enableReading() {
                events_ |= kReadEvent;
                update();       //epoll_ctl(epfd, EPOLL_CTL_MOD, fd, {events_, fd});
            }
            void disableReading() {
                events_ &= ~kReadEvent;
                update();
            }
            void enableWriting() {
                events_ |= kWriteEvent;
                update();
            }
            void disableWriting() {
                events_ &= ~kWriteEvent;
                update();
            }
            void disableAll() {
                events_ = kNoneEvent;
                update();
            }

            EventLoop* ownerLoop() const {
                return loop_;
            }
            void remove();

        private:
            void update();
            void handleEventWithGuard(Timestamp receiveTime);


            static const int kNoneEvent;
            static const int kReadEvent;
            static const int kWriteEvent;

            EventLoop *loop_;   //事件循环
            const int fd_;      //Poller监听的文件描述符，对于一个Channel类来讲是固定的
            int events_;        //注册的事件类型
            int revents_;       //返回的事件类型
            int index_;         //channlList的顺序应该是

            std::weak_ptr<void> tie_;
            bool tied_;
            //TODO:没有muduo库的eventHandling_、addedToLoop_
            //事件回调
            ReadEventCallback  readCallback_;
            EventCallback writeCallback_;
            EventCallback closeCallback_;
            EventCallback errorCallback_;
        };

    }
}


#endif //SUB_MUDUO_CHANNEL_H
