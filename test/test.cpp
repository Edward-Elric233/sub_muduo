#include "test.h"
#include "Timestamp.h"
#include "Logger.h"
#include "InetAddress.h"
#include "BlockingQueue.h"

#include <iostream>

using namespace std;

void test_Timestamp() {
    cout << sub_muduo::Timestamp::now().toFormattedString() << endl;
}

void test_Logger() {
    sub_muduo::LOG_TRACE("Test Trace");
    sub_muduo::LOG_INFO("Test INFO");
    sub_muduo::LOG_WARN("Test WARN");
    sub_muduo::LOG_ERROR("Test ERROR");
    sub_muduo::LOG_FATAL("Test FATAL");
}

void test_InetAddress() {
    sub_muduo::net::InetAddress addr1(8080);
    cout << addr1.getIpPort() << endl;
    sub_muduo::net::InetAddress addr2("127.0.0.1", 8888);
    cout << addr2.getIpPort() << endl;
}
