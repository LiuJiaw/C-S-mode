#ifndef TCPSER_H_INCLUDED
#define TCPSER_H_INCLUDED
#include <iostream>
#include <windows.h>
#include <winbase.h>
#include <vector>
#include <map>
#include <string>
#include <process.h>
#include "Class.h"

using namespace std;
#pragma comment(lib, "ws2_32.lib")			//动态库函数

#define SERVERIP "127.0.0.1"//服务器TCP的IP地址
#define SERVERPORT 6666		 //服务器TCP端口
#define CONN_NUM 10              //连接客户端数量

#define TIMEFOR_THREAD_HELP			1500	//清理资源线程退出时间
#define TIMEFOR_THREAD_EXIT			5000	//主线程睡眠时间

typedef vector<CClient*> ClIENTVECTOR;		//向量容器
typedef vector<string> SVECTOR;             //内容字符


/**
 * 全局变量
 */
extern char	dataBuf[MAX_NUM_BUF];				//写缓冲区
extern bool	bConning;							//与客户端的连接状态
extern bool bSend;                              //发送标记位
extern bool clientConn;                         //连接客户端标记
extern SOCKET sServer;							//服务器监听套接字
extern HANDLE AccHandle;						//数据处理线程句柄
extern HANDLE CleanHandle;						//数据接收线程

/**
 *函数申明
 */
bool initSever(void);                       //初始化
void InitGlobal(void);
bool initSocket(void);						//初始化非阻塞套接字
void exitServer(void);						//释放资源
bool startService(void);					//启动服务器
void inputAndOutput(void);                  //处理数据
void showServerStartMsg(bool bSuc);         //显示错误信息
void showServerExitMsg(void);               //显示退出消息
void handleData(char* str);                 //数据处理
void showTipMsg(int bFirstInput);          //显示输入提示信息
bool createCleanAndAcceptThread(void);      //开启监控函数
DWORD __stdcall acceptThread(void* pParam); //开启客户端请求线程
DWORD __stdcall cleanThread(void* pParam);

#endif // TCPSER_H_INCLUDED
