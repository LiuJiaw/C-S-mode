#ifndef TCPSER_H_INCLUDED
#define TCPSER_H_INCLUDED
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <process.h>
#include "SClient.h"

using namespace std;
#pragma comment(lib, "ws2_32.lib")			//动态库函数

#define SERVERIP "127.0.0.1"//服务器TCP的IP地址
#define SERVERPORT 6666		 //服务器TCP端口
#define CONN_NUM 10              //连接客户端数量

#define TIMEFOR_THREAD_HELP			1500	//清理资源线程退出时间
#define TIMEFOR_THREAD_EXIT			5000	//主线程睡眠时间

typedef vector<SClient*> ClIENTVECTOR;		//向量容器

//初始化全局变量
void InitGlobal(void);
//初始化非阻塞套接字
bool initSocket(void);
//退出服务器
void Exit(void);
//获取输入信息
void SendBuffer(void);
//传递数据给各客户端
void ToAllCliBuf(char* str, char* addr);
//显示服务器启动信息
void showServerStartMsg(bool bSuc);
//显示退出消息
void showServerExitMsg(void);
//创建线程，进行监听和清除操作
bool createThread(void);
//接受客户端请求线程执行函数
DWORD __stdcall acceptThread(void* pParam);
//清理已断开连接客户端的线程执行函数
DWORD __stdcall cleanThread(void* pParam);

#endif // TCPSER_H_INCLUDED
