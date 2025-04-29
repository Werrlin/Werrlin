



项目构建：

```
编译调试工具
GCC、CMake
GDB、LLDB、Valgrind、ASan
性能分析工具
perf、VTune、火焰图生成
代码优化技巧（内联汇编、缓存友好设计、分支预测）
协作工具
版本控制（Git）、CI/CD（Jenkins、GitLab CI）、代码审查（Gerrit）
```

核心 C++ 技能

```
现代C++特性
熟练使用C++11/14/17/20核心特性（如智能指针、lambda表达式、移动语义、constexpr、concepts、coroutines等）。
理解RAII（资源获取即初始化）和异常安全编程。
模板元编程（TMP）、SFINAE、CRTP等高级技术。

内存管理
堆/栈内存区别、内存对齐、内存泄漏检测与防范。
智能指针（unique_ptr, shared_ptr, weak_ptr）及自定义内存分配器。
内存碎片优化策略（如Slab分配器）。

多线程与并发编程
熟悉std::thread、互斥锁（std::mutex）、条件变量、原子操作（std::atomic）。
掌握无锁编程、线程池设计，了解C++20协程（Coroutines）。
线程局部存储（TLS）、std::barrier（C++20屏障同步）、std::latch。
硬件并发原语：CAS（Compare-And-Swap）、内存屏障（Memory Barriers）。

数据结构和算法
手写常见数据结构（红黑树、哈希表、跳表）及算法（排序、动态规划、图算法）。
熟悉STL容器内部实现（如vector扩容机制、unordered_map的哈希冲突处理）。 
并行算法（C++17起STL支持的std::for_each并行执行）、跳表（SkipList）的并发优化版本。
```

系统设计与架构

```
设计模式与范式
	单例模式（线程安全实现）
    常用设计模式（工厂、观察者、策略、装饰器）及C++实现。
    面向对象设计（SOLID原则）、泛型编程、函数式编程思想。
系统级开发
    操作系统原理（进程/线程调度、虚拟内存、文件系统）。
    网络编程（Socket、TCP/IP、HTTP/2、QUIC协议）。
    性能调优（缓存优化、CPU流水线、SIMD指令集、代码剖析（Profiling）、热点分析、编译器优化选项（如-O3、-march=native）。
    信号处理（如SIGSEGV捕获）、进程间通信（IPC）（共享内存、消息队列、信号量）。

分布式与高并发
    分布式系统基础（CAP定理、RPC框架如gRPC）。
    高吞吐/低延迟系统设计（如高频交易、实时流处理）。
```

网络

```
TCP/UDP协议核心差异
    TCP：面向连接、可靠传输（三次握手、流量控制、拥塞控制、四次挥手）。
    UDP：无连接、低延迟但不可靠（适合实时音视频、游戏同步）。
    理解头部结构（如TCP的序列号、ACK机制，UDP的校验和）。
    TCP/UDP/QUIC协议
Socket API
    socket(), bind(), listen(), accept(), connect(), send(), recv(), sendto(), recvfrom(), setsockopt()
	理解阻塞/非阻塞模式、同步/异步I/O的区别。
地址处理
	IPv4/IPv6兼容：struct sockaddr_in vs struct sockaddr_in6。
	地址转换函数：inet_pton(), inet_ntop(), getaddrinfo()。
	网络字节序转换：htonl(), htons(), ntohl(), ntohs()。
I/O多路复用
	掌握select()、poll()、epoll（Linux）或kqueue（BSD/Mac）的原理与使用。
	基于事件驱动的编程模型（Reactor模式）。
	对比水平触发与边缘触发。
异步I/O与协程
    使用C++20协程结合异步Socket（如asio::awaitable）。
    基于Boost.Asio或独立asio库实现异步网络服务。
    理解Proactor模式与Reactor模式的区别。
协议设计与解析
    自定义二进制协议（TLV格式、包头设计、序列化/反序列化）。
    常见协议实现：HTTP/1.1（解析请求头、分块传输）、WebSocket握手。
    使用Protobuf/FlatBuffers优化网络数据传输。
粘包&应用层协议分包
安全与加密
   	SSL/TLS（如OpenSSL库）、DTLS（基于UDP的安全传输）。
    网络安全防护：DDoS缓解策略、流量限速（Rate Limiting）。
协议与框架
    HTTP/3（基于QUIC）、MQTT 5.0（物联网协议）、Thrift（Facebook的RPC框架）。
    网络攻击防范：SYN Flood防御、TLS握手优化。
```

C++ 库

```
DPDK
协程调度器
io_uring
redlock
log4cpp
protobuf
gRPC/bRPC
redis
MySQL
kafka
MQTT
nginx
常用库扩展
	测试框架：Google Test、Catch2（单元测试）、Mockito（模拟测试）。
    序列化库：MessagePack、Cap'n Proto（零拷贝序列化）。
    消息中间件：ZeroMQ、NATS（轻量级消息队列）。
其他工具库
Boost.Beast（HTTP/WebSocket实现）、Folly（Facebook高性能库）、Abseil（Google基础库）。
```

项目优化

```
内存池
线程池
数据库连接池
使用消息队列的方式实现读写分离解耦合
无锁消息队列
MQ原理
定时器	时间轮算法（Timing Wheel）、最小堆调度（如std::priority_queue实现定时任务）。
```

分布式

云原生