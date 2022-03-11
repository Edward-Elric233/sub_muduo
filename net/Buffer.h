// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/12/18
// Description: 
#ifndef SUB_MUDUO_BUFFER_H
#define SUB_MUDUO_BUFFER_H

#include <copyable.h>
#include <Types.h>

#include <vector>
#include <algorithm>

namespace sub_muduo {
    namespace net {

        class Buffer : copyable {
        public:
            static constexpr size_t kCheapPrepend = 8;
            static constexpr size_t kInitialSize = 1024;

            explicit Buffer(size_t initialSize = kInitialSize)
                : buffer_(kCheapPrepend + initialSize)
                , readerIndex_(kCheapPrepend)
                , writerIndex_(kCheapPrepend) {}

            size_t readableBytes() const {
                return writerIndex_ - readerIndex_;
            }
            size_t writableBytes() const {
                return buffer_.size() - writerIndex_;
            }
            size_t prependableBytes() const {
                return readerIndex_;
            }
            char *begin() {
                return buffer_.data();
            }
            const char *begin() const {
                return buffer_.data();
            }
            //返回缓冲区中可读数据的首地址
            const char *peek() const {
                return begin() + readerIndex_;
            }
            void retrieve(size_t len) {
                if (len < readableBytes()) {
                    readerIndex_ += len;
                } else {
                    retrieveAll();
                }
            }
            void retrieveAll() {
                readerIndex_ = kCheapPrepend;
                writerIndex_ = kCheapPrepend;
            }
            string retrieveAsString(size_t len) {
                string result(peek(), len);
                retrieve(len);
                return result;
            }
            string retrieveAllAsString() {
                return retrieveAsString(readableBytes());
            }
            //确认可写缓冲区的长度大于len
            void ensureWriteableBytes(size_t len) {
                if (writableBytes() < len) {
                    makeSpace(len);
                }
            }
            void ensureWritable
            void append(const char *data, size_t len) {

            }
        private:
            //扩容函数
            void makeSpace(size_t len) {
                if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
                    buffer_.resize(writerIndex_ + len);
                } else {
                    size_t readable = readableBytes();
                    std::copy(begin() + readerIndex_, begin() + writerIndex_, begin() + kCheapPrepend);
                    readerIndex_ = kCheapPrepend;
                    writerIndex_ = readerIndex_ + readable;
                }
            }
            std::vector<char> buffer_;
            size_t readerIndex_;
            size_t writerIndex_;
        };

    }
}


#endif //SUB_MUDUO_BUFFER_H
