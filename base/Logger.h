// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/26
// Description: 
#ifndef SUB_MUDUO_LOGGER_H
#define SUB_MUDUO_LOGGER_H

#include "Types.h"
#include "Timestamp.h"

#include <iostream>
#include <initializer_list>

namespace sub_muduo {

    class Logger {
    public:
        enum LogLevel {
            TRACE,  //追踪
            DEBUG,  //调试
            INFO,   //重要信息
            WARN,   //警告
            ERROR,  //错误
            FATAL,  //致命错误
            NUM_LOG_LEVELS, //用来保存LogLevel的种类
        };
        static constexpr char logLevelName[NUM_LOG_LEVELS][6] = {
                "TRACE",  //追踪
                "DEBUG",  //调试
                "INFO",   //重要信息
                "WARN",   //警告
                "ERROR",  //错误
                "FATAL",  //致命错误
        };
        //获取logLevel
        static LogLevel getLogLevel();
        //设置logLevel
        static void setLogLevel(LogLevel level);


        /*!
         * 输出YYYY-MM-DD HH:MM:SS [logLevel] msg [filename:line]
         * TODO:目前就输出到标准输出，muduo库是写到一个自己写的stream中，有点复杂，日后再完善
         * @param msg
         */
        void log(const string &msg);

        class FileName;

        Logger(FileName file, int line, LogLevel level = INFO);
        //在析构函数刷新缓冲区
        ~Logger();


        /*!
         * 作用同muduo库中SourceFile，从__FILE__提取出文件名
         */
        class FileName {
        public:
            const char *name_;
            int size_;

            /*!
             * 不用声明成explicit
             * @tparam N char数组长度
             * @param file __FILE__
             */
            template<int N>
            FileName(const char (&file)[N])
                    : name_(file)
                    , size_(N - 1){
                //从__FILE__中提取出文件名
                const char *slash = strrchr(name_, '/');
                if (slash != nullptr) {
                    name_ = slash + 1;
                    size_ -= static_cast<int>(name_ - file);    //减去目录部分的长度
                }
            }
        };

    private:
        static LogLevel logLevel; //使用全局变量保存日志等级
        /*!
         * 嵌套类：日志输出的实现部分
         * TODO:目前还没有搞懂为什么要将实现部分抽取出来
         */
        class Impl {
        public:
            Timestamp time_;
            std::ostream  &stream_ = std::cout;  //TODO:这里使用ostream代替Logstream，会初始化为cout
            LogLevel level_;
            int line_;
            FileName name_;
            //TODO:不知道old_errno是做什么的
            Impl(const FileName &file, int line, LogLevel level, int old_errno = 0);


        };

        Impl impl_;
    };

    inline void LOG_TRACE(const string &msg) {
        if (Logger::getLogLevel() <= Logger::TRACE) {
            Logger(__FILE__, __LINE__, Logger::TRACE).log(msg);
        }
    }
    inline void LOG_DEBUG(const string &msg) {
        if (Logger::getLogLevel() <= Logger::DEBUG) {
            Logger(__FILE__, __LINE__, Logger::DEBUG).log(msg);
        }
    }
    inline void LOG_INFO(const string &msg) {
        if (Logger::getLogLevel() <= Logger::INFO) {
            Logger(__FILE__, __LINE__, Logger::INFO).log(msg);
        }
    }
    inline void LOG_WARN(const string &msg) {
        if (Logger::getLogLevel() <= Logger::WARN) {
            Logger(__FILE__, __LINE__, Logger::WARN).log(msg);
        }
    }
    inline void LOG_ERROR(const string &msg) {
        if (Logger::getLogLevel() <= Logger::ERROR) {
            Logger(__FILE__, __LINE__, Logger::ERROR).log(msg);
        }
    }

    inline void LOG_FATAL(const string &msg) {
        if (Logger::getLogLevel() <= Logger::FATAL) {
            Logger(__FILE__, __LINE__, Logger::FATAL).log(msg);
        }
    }

}



#endif //SUB_MUDUO_LOGGER_H
