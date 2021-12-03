// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/26
// Description: 
#include "Logger.h"

#include <cstdlib>


namespace sub_muduo {

    constexpr char Logger::logLevelName[NUM_LOG_LEVELS][6];

    /*!
     * 在环境变量中检查是否定义日志等级为TRACE/DEBUG，否则就定义为INFO
     * @return
     */
    Logger::LogLevel initLogLevel() {
        if (::getenv("SUB_MUDUO_LOG_TRACE")) return Logger::TRACE;
        else if (::getenv("SUB_MUDUO_LOG_DEBUG")) return Logger::DEBUG;
        else return Logger::INFO;
    }

    Logger::LogLevel Logger::logLevel = initLogLevel();


    Logger::LogLevel Logger::getLogLevel() {
        return logLevel;
    }

    void Logger::setLogLevel(LogLevel level) {
        logLevel = level;
    }

    void Logger::log(const string &msg) {
        impl_.stream_ << impl_.time_.toFormattedString()
                      << " [" << logLevelName[impl_.level_] << "] "
                      << msg
                      << " [in " << impl_.name_.name_ << ":" << impl_.line_ << "]\n";
    }


    Logger::Impl::Impl(const FileName &file, int line, LogLevel level, int old_errno)
            : name_(file)
            , line_(line)
            , level_(level)
            , time_() {}

    Logger::Logger(FileName file, int line, LogLevel level)
            : impl_(file, line, level) {}

    Logger::~Logger() {
        impl_.stream_ << std::flush;
    }
}