#Customed Telnet Server And Client

####描述
这是sockets编程写的一个简单的telnet服务器，分为服务器和客户端两部分。服务器采用多线程异步套接字架构，对于来自客户端的连接会触发SIGIO信号，并将此连接交由新线程来处理。对于每个连接的客户端，服务器会维护一个套接字列表，SIGINT信号会触发服务器向所有在线的客户端发送一个LEAVE消息实现graceful-shutdown。客户端同样监听SIGINT信号，在客户端下线之前向服务器发送一个LEAVE消息。此外，还支持文本型的文件传送、服务器端支持日志记录、采用了简单的Ascii码循环移位加密、支持用户名和密码的登录认证等等。

####编译
- 服务器 `make` 会在目录下生成server二进制文件
- 客户端 `make` 会在目录下生成client二进制文件

####使用
- 服务器  `./server port username password`
- 客户端  `./client domain/ip port username password [file]`

####功能

######服务器支持多线程异步套接字架构
可支持多客户端同时连接且会话都为非阻塞模式，无需等待
######监听SIGIO,SIGINT,SIGPIP信号
SIGIO信号提供非阻塞支持，SIGINT信号提供紧急中断支持，SIGPIP信号提供异常下线支持
######支持字符文件传送
可以传送字符型文件
######支持加密
支持加密模块，可以应用各种加密方法。
######支持graceful leave
服务器、客户端下线前会发送leave消息，实时通知对方
######支持日志记录
服务器会记录客户端的登陆情况、执行的命令等。





