#ifndef TCPCLI_H_INCLUDED
#define TCPCLI_H_INCLUDED

#include <iostream>
#include <windows.h>
#include <process.h>

#pragma comment(lib, "WS2_32.lib")
using namespace std;

//服务器IP
#define	SERVERIP "127.0.0.1"
//服务器TCP端口
#define	SERVERPORT 6666


//缓冲区的最大长度
#define	BUF_SIZE 100
//尝试连接最大失败次数
#define LIMIT_FAIL 10
#define EXITTIME 3000

#define InitSocketFail 1
#define ConnectServerSuccess 2
#define ConnectServerFail 3
#define CreateThreadSuccess 4
#define CreateThreadFail 5
#define ServerClose 6

//初始化全局变量
void InitGlobal(void);
//非阻塞套接字
bool InitSocket(void);
//连接服务器
bool ConnectServer(void);
//打印信息
void ShowMessage(int stateCode);
//创建收发数据的线程
bool CreateThread(void);
//接受数据线程的执行函数
DWORD __stdcall	RecvThread(void*);
//发送数据线程的执行函数
DWORD __stdcall	SendThread(void*);

void WriteBuffer();
//退出客户端
void Exit(int exittime);

#endif // TCPCLI_H_INCLUDED
