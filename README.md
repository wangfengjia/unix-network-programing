# unix网络编程索引目录
### 第一章-简介
1. 概述
2. 一个简单的时间获取客户程序
3. 协议无关性
4. 错误处理：包裹函数
5. 一个简单的时间获取服务程序
6. 本书中客户/服务器程序示例索引表
7. OSI模型
8. BSD网络支持历史
9. 测试用网络及主机
10. Unix标准
11. 64位体系结构

### 第二章-传输层：TCP、UDP和SCTP
1. 概述
2. 总图
3. 用户数据报协议(UDP)
4. 传输控制协议(TCP)
5. 流控制传输协议(SCTP)
6. TCP连接的建立和终止
7. TIME_WAIT状态
8. SCTP关联的建立和终止
9. 端口号
10. TCP端口号与并发服务器
11. 缓冲区大小及限制
12. 标准因特网服务
13. 常见因特网应用的协议使用

### 第三章-套接字编程简介
1. 概述
2. 套接字地址结构
3. 值-结果参数
4. 字节排序函数
5. 字节操纵函数
6. inet_aton、inet_addr和inet_ntoa函数
7. inet_pton函数和inet_ntop函数
8. sock_ntop和相关函数
9. readn、writen和readline函数

### 基本TCP套接字编程
1. 概述
2. socket函数
3. connect函数
4. bind函数
5. listen函数
6. accept函数
7. fork和exec函数
8. 并发服务器
9. close函数
10. getsockname和getperrname函数

### TCP客户/服务器程序示例
1. 概述
2. TCP服务器程序:main函数
3. TCP服务器程序:str_echo函数
4. TCP客户端程序:main函数
5. TCP客户端程序:str_cli函数
6. 正常启动
7. 正常终止
8. POSIX信号处理
9. 处理SIGCHLD信号
10. wait和waitpid函数
11. accept返回前连接终止
12. 服务器进程终止
13. SIGPIPE信号
14. 服务器主机崩溃
15. 服务器主机崩溃后重启
16. 服务器主机关机
17. TCP程序例子小结
18. 数据格式

### I/O复用:select和poll函数
1. 概述
2. I/O模型
3. select函数
4. str_cli函数(修订版)
5. 批量输入
6. shutdown函数
7. str_cli函数(再修订版)
8. TCP回射服务器程序(修订版)
9. pselect函数
10. poll函数
11. TCP回射服务器程序(再修订版)


### 套接字选项
1. 概述
2. getsockopt和setsockopt函数
3. 检查选项是否受支持并获取默认值
4. 套接字状态
5. 通用套接字选项
6. IPv4套接字选项
7. ICMIPv6套接字选项
8. IPv6套接字选项
9. TCP套接字选项
10. SCTP套接字选项
11. fcntl函数


















