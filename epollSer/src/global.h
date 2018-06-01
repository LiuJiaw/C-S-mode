/*
 * global.h
 *
 *  Created on: 2018年5月31日
 *      Author: garvin
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

//服务器TCP的IP地址
#define SERVERIP "127.0.0.1"
//服务器TCP端口
#define SERVERPORT 6666
//连接客户端数量
#define CONN_NUM 10
//设置epoll容量
#define EPOLL_SIZE 10
//设置无效套接字
#define INVALID_SOCKET -1
//设置函数返回错误码
#define SOCKET_ERROR -1
//缓冲区的规格
#define BUF_SIZE 1024

//监听套接字
int epSer;
//epoll专用的描述符
int epFd;


//初始化全局变量
void InitGlobal();
//初始化套接字
bool InitSocket();
//
bool StartEpoll();
//
void Epoll();
//退出服务器
void EXIT();

#endif /* GLOBAL_H_ */
