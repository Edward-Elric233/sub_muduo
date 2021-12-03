// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/24
// Description: 创建noncopyable接口，使得派生类无法进行拷贝构造和拷贝赋值
#ifndef SUBMUDUO_NONCOPYABLE_H
#define SUBMUDUO_NONCOPYABLE_H

namespace sub_muduo {

/*!
 * 使得派生类无法进行拷贝构造和拷贝赋值
 */
    class noncopyable {
    public:
        noncopyable(const noncopyable&) = delete;
        noncopyable& operator =(const noncopyable&) = delete;

    protected:
        //不影响构造和析构
        noncopyable() = default;
        ~noncopyable() = default;
    };

}

#endif //SUBMUDUO_NONCOPYABLE_H
