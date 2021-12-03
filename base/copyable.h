// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/25
// Description: 创建copyable接口，允许派生类拷贝
#ifndef SUBMUDUO_COPYABLE_H
#define SUBMUDUO_COPYABLE_H

namespace sub_muduo {

    class copyable {
    public:
        copyable() = default;
        ~copyable() = default;
    };

}
#endif //SUBMUDUO_COPYABLE_H
