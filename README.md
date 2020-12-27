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

### 基本UDP套接字编程
1. 概述
2. recvfrom和sendto函数
3. UDP回射服务器程序: main函数
4. UDP回射服务器程序: dg_echo函数
5. UDP回射客户端程序: main函数
6. UDP回射客户端程序: dg_cli函数
7. 数据报的丢失
8. 验证收到的响应
9. 服务器进程未运行
10. UDP程序例子小结
11. UDP的connect函数
12. dg_cli函数
13. UDP缺乏流量控制
14. UDP中的外出接口的确定
15. 使用select函数的TCP和UDP服务器

### 名字与地址转换
1. 概述
2. 域名系统
3. gethostbyname函数
4. gethostbyaddr函数
5. getservbyname和getservbyport函数
6. getaddrinfo函数
7. gai_strerror函数
8. freeaddrinfo函数
9. getaddrinfo函数: IPv6
10. getaddrinfo函数: 例子
11. host_serv函数
12. tcp_connect函数
13. tcp_listen函数
14. udp_client函数
15. udp_connect函数
16. udp_server函数
17. getnameinfo函数
18.可重入函数
19. gethostbyname_r和gethostbyaddr_r函数
20. 作废的IPv6地址解析函数
21. 其他网络相关信息

### IPv4和IPv6的互操作性
1. 概述
2. IPv4客户和IPv6服务器
3. IPv4客户和IPv4服务器
4. IPv6地址测试宏
5. 源代码的可移植性

### 守护进程和inetd超级服务器
1. 概述
2. syslogd守护进程
3. syslog函数
4. daemon_init函数
5. inetd守护进程
6. daemon_inetd函数

### 高级I/O函数
1. 概述
2. 套接字超时
3. recv和send函数
4. readv和writev函数
5. 辅助数据
6. 排队的数据量
7. 套接字和标准I/O
8. 高级轮询技术
9. T/TCP：事务目的TCP

### 非阻塞式I/O
1. 概述
2. 非阻塞读和写:str_cli函数(修订版)
3. 非阻塞connect
4. 非阻塞connect:时间获取客户程序
5. 非阻塞connect:Web客户程序
6. 非阻塞accept


### ioctl操作
1. 概述
2. ioctl函数
3. 套接字操作
4. 文件操作
5. 接口配置
6. get_ifi_info函数
7. 接口操作
8. ARP高速缓存操作
9. 路由表操作

### 路由套接字
1. 概述
2. 数据链路套接字地址结构
3. 读和写
4. sysctl操作
5. get_ifi_info函数
6. 接口名字和索引函数

### 密钥管理套接字
1. 概述
2. 读和写
3. 倾泻安全关联数据库
4. 创建静态安全关联
5. 动态维护安全关联

### 高级UDP套接字编程
1. 概述
2. 接收标志、目的地址和接收索引
3. 数据报截断
4. 何时用UDP代替TCP
5. 给UDP应用增加可靠性
6. 捆绑接口地址
7. 并发UDP服务器
8. IPv6分组信息
9. IPv6路径MTU控制

### 高级SCTP套接字编程
1. 概述
2. 自动关闭的一到多式服务器程序
3. 部署递送
4. 通知
5. 无序的数据
6. 捆绑地址子集
7. 确定对端和本端地址信息
8. 给定IP地址找出关联ID
9. 心搏和地址不可达
10. 关联剥离
11. 定时控制
12. 何时改为用SCTP代替TCP

### 带外数据
1. 概述
2. TCP带外数据
3. sockatmark函数
4. TCP带外数据总结
5. 客户/服务器心博函数

### 信号驱动式I/O
1. 概述
2. 套接字的信号驱动I/O
3. 使用SIGIO的UDP回射服务器程序


### 线程
1. 概述
2. 基本线程函数:创建和终止
3. 使用线程的str_cli函数
4. 使用线程的TCP回射服务器程序
5. 线程特定数据
6. web客户与同时连接
7. 互斥锁
8. 条件变量
9. web客户与同时连接

### IP选项
1. 概述
2. IPv4选项
3. IPv4源路径选项
4. IPv6扩展首部
5. IPv6步跳选项和目的地选项
6. IPv6路由首部
7. IPv6粘附选项
8. 历史性IPv6高级API

### 原始套接字
1. 概述
2. 原始套接字创建
3. 原始套接字输出
4. 原始套接字输入
5. ping程序
6. traceroute程序
7. 一个ICMP消息守护程序

### 数据链路访问
1. 概述
2. BPF:BSD分组过滤
3. DLPI:数据链路提供者接口
4. Linux: SOCK_PACKET和PF_PACKET
5. libpcap:分组捕获函数库
6. libnet:分组构造和输出函数库
7. 检查UDP的检验和字段

### 客户/服务器程序设计范式
1. 概述
2. TCP客户程序设计范式
3. TCP测试用户客户程序
4. TCP迭代服务器程序
5. TCP并发服务器-为每个客户请求fork一个子进程
6. TCP并发服务器-子进程池，每个子进程无保护地调用accept
7. TCP并发服务器-子进程池，使用文件上锁保护调用accept
8. TCP并发服务器-子进程池，使用线程互斥锁上锁保护accept
9. TCP并发服务器-子进程池，父进程使用UNIX域套接字向子进程传递已连接套接字描述符
10. TCP并发服务器-为每个客户请求创建一个线程
11. TCP并发服务器-线程池，使用互斥锁上锁保护accept
12. TCP并发服务器-线程池，主线程调用accept获取到新的已连接套接字，其他线程中的一个空闲线程处理这个连接上的请求

### 流
1. 概述
2. 概貌
3. getmsg和putmsg函数
4. getpmsg和putpmsg函数
5. ioctl函数
6. TPI:传输提供者接口


















