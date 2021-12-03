// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/25
// Description: 
#include "Timestamp.h"

namespace sub_muduo {

//TODO:编译时检查，虽然目前还没有明白为什么要检查，目前觉得可能是顺手写的哈哈
    static_assert(sizeof (Timestamp) == sizeof (int64_t),
                  "Timestamp应该和int64_t一样大");

    const Timestamp Timestamp::now() {
        return Timestamp();
    }

    string Timestamp::toFormattedString() const {
        constexpr int BUFSIZE = 32;
        char buf[BUFSIZE] = {0};
        time_t t = system_clock::to_time_t(tp_);    //转换成C提供的时间类型time_t，相当于time()函数
        struct tm tm_time;
        //使用localtime获得当前时区的时间，muduo库使用的是gmtime，这个是标准时间
        //使用可重入的_r版本
        localtime_r(&t, &tm_time);
        //格式化输出YYYY-MM-DD HH:MM:SS
        snprintf(buf, BUFSIZE, "%4d-%02d-%02d %02d:%02d:%02d", tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        return buf;
    }

}
