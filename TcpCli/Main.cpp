#include <iostream>
#include "TcpCli.h"

using namespace std;

int main()
{
    //初始化全局变量
    InitGlobal();
    //初始化套接字
	int initstate=InitSocket();
	if(!initstate)
    {
        ShowMessage(InitSocketFail);
        Exit(EXITTIME);
        return -1;
    }


	//尝试连接服务器，ConnectServer返回true表示连接成功
	int connectstate=ConnectServer();
	if(connectstate)
		ShowMessage(ConnectServerSuccess);
	else
    {
		ShowMessage(ConnectServerFail);
        Exit(EXITTIME);
        return -1;
	}

	//创建收发数据线程，CreateThread返回true表示创建成功
	int Createstate=CreateThread();
	if(!Createstate)
    {
        ShowMessage(CreateThreadFail);
        Exit(EXITTIME);
        return -1;
    }

	//用户输入数据和显示结果
	WriteBuffer();

	return 0;
}
