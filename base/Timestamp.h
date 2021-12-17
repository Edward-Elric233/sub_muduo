// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/25
// Description: 时间类，使用chrono库进行重写
#ifndef SUBMUDUO_TIMESTAMP_H
#define SUBMUDUO_TIMESTAMP_H

#include "copyable.h"
#include "Types.h"

#include <chrono>
#include <string>

namespace sub_muduo{


    class Timestamp :  copyable {
        using system_clock = std::chrono::system_clock;
        using string = std::string;
        system_clock::time_point tp_;    //保存当前时间点
    public:
        //获取当前时间
        static const Timestamp now();
        /*!
         * 显示构造函数
         * @param tp 默认传递当前系统时间
         */
        explicit Timestamp(const system_clock::time_point &tp = system_clock::now())
                : tp_(tp) {}

        /*!
         * 转换成格式化字符串输出
         * TODO::目前不进行格式控制，后面有时间学习一下Log4cpp的格式控制
         * @return
         */
        string toFormattedString() const;
    };

}
#endif //SUBMUDUO_TIMESTAMP_H

