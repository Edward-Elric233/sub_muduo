# sub_muduo库
阅读陈硕大神的《Linux 多线程服务端编程》模仿`muduo`实现的网络库

很多地方都进行了简化处理，已经使用`TODO`进行标注。同时对代码进行了详细的中文注释，更加适合初学者阅读。
代码的组织结构与`muduo`库类似：
- 系统编程/多线程编程相关的代码主要放在`base`目录下，生成`sub_muduo_base`动态库
- 网络编程相关的代码主要放在`net`目录下，生成`sub_muduo_net`动态库

每个文件的作用在头部已经进行了简单的说明