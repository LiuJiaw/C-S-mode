# C-S-mode
简单的Client/Server模型，使用tcp协议
 

能够实现多个客户端同时与服务器进行通信，服务器端能够向所有客户端广播数据。


服务器端与客户端可同时连续收发数据，客户端连接断开时服务器会有相应提示，服务器断开则客户端自动退出


使用时，需将TcpCli.h与TcpSer.h中的服务器IP地址，改为服务器端主机的公网IP地址；此外需保证端口一致。

实现客户端消息群发功能
 
 会持续更新 ...
