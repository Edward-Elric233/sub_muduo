cmake_minimum_required(VERSION 3.20)
project(sub_muduo)

set(CMAKE_CXX_STANDARD 20)

# SubMuduo最终编译成so动态库，设置动态库的路径，放在lib目录下
set(CMAKE_LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

#设置头文件目录
include_directories(net)
include_directories(base)
#设置库文件目录
link_directories(lib)

aux_source_directory(base BASE_LIST)
aux_source_directory(net NET_LIST)
aux_source_directory(net/poller POLLER_LIST)

# 编译生成动态库sub_muduo
add_library(sub_muduo_base SHARED ${BASE_LIST})
add_library(sub_muduo_net SHARED ${NET_LIST} ${POLLER_LIST})

add_executable(test test/main.cpp test/test.cpp)

target_link_libraries(test sub_muduo_net)
target_link_libraries(test sub_muduo_base)
target_link_libraries(test pthread)

