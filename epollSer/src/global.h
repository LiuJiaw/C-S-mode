/*
 * global.h
 *
 *  Created on: 2018年5月31日
 *      Author: garvin
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <vector>
#include <string.h>

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

using namespace std;

//监听套接字
extern int epSer;
//epoll专用的描述符
extern int epFd;
//记录已连接客户端套接字的容器
extern vector<int> clisockets;
//发送缓冲区
extern char sendBuf[BUF_SIZE];
//接收缓冲区
extern char recvBuf[BUF_SIZE];

//初始化全局变量
void InitGlobal();
//初始化套接字
bool InitSocket();
//
bool StartEpoll();
//退出服务器
void Exit();
//
void Epoll();
//接受服务器连接
void AcceptConn();
//获取连接客户端的消息,返回值为所得信息的长度，参数为客户端套接字
int GetMessage(int);
//向客户端广播消息
void Boardcast(int);
//删除客户端连接
void RemoveConn(int);

#endif /* GLOBAL_H_ */
