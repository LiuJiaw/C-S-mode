#include <process.h>
#include <stdio.h>
#include "Class.h"
#include "TcpSer.h"


//构造函数
SClient::SClient(const SOCKET sClient, const sockaddr_in &addrClient)
{
	//初始化变量
	m_RecvHandle=NULL;
	m_SendHandle=NULL;
	m_socket=sClient;
	m_addr=addrClient;
	m_bConning=false;
	memset(BufSend, 0, MAX_NUM_BUF);
}
//析构函数
SClient::~SClient()
{
	closesocket(m_socket);			//关闭套接字
	m_socket=INVALID_SOCKET;		//套接字无效
}

//创建发送和接收数据线程
bool SClient::StartRuning(void)
{
	m_bConning=true;//设置连接状态

	//创建接收数据线程
	unsigned long ulThreadId;
	m_RecvHandle = CreateThread(NULL, 0, RecvThread, this, 0, &ulThreadId);
	if(m_RecvHandle==NULL)
	{
		return false;
	}
	else
    {
		CloseHandle(m_RecvHandle);
	}

	//创建接收客户端数据的线程
	m_SendHandle=CreateThread(NULL, 0, SendThread, this, 0, &ulThreadId);
	if(m_SendHandle==NULL)
	{
		return false;
	}
	else
    {
		CloseHandle(m_SendHandle);
	}

	return true;
}


//接收客户端数据
DWORD  SClient::RecvThread(void* pParam)
{
	SClient *pClient=(SClient*)pParam;	//客户端对象指针
	int		reVal;							//返回值
	char	temp[MAX_NUM_BUF];				//临时变量


	while(pClient->m_bConning)				//连接状态
	{
	    memset(temp, 0, MAX_NUM_BUF);
		reVal = recv(pClient->m_socket, temp, MAX_NUM_BUF, 0);	//接收数据

		//处理错误返回值
		if (reVal==SOCKET_ERROR)
		{
			int nErrCode=WSAGetLastError();

			if (nErrCode==WSAEWOULDBLOCK)	//接受数据缓冲区不可用
			{
				continue;						//继续循环
			}
			//客户端关闭了连接
			else if (nErrCode==WSAENETDOWN ||nErrCode==WSAETIMEDOUT ||nErrCode==WSAECONNRESET)
			{
				break;							//线程退出
			}
		}

		//客户端关闭了连接
		if ( reVal == 0)
		{
			break;
		}

		//收到数据
		if (reVal > 0)
		{
		    char *pClientIP = inet_ntoa(pClient->m_addr.sin_addr);
            u_short  clientPort = ntohs(pClient->m_addr.sin_port);
			cout<<"IP: "<<pClientIP<<"\t向服务器发送数据："<<temp<<endl;      //输出显示数据
		}
	}
	pClient->m_bConning = false;			//与客户端的连接断开
	return 0;								//线程退出
}

//向客户端发送数据
DWORD SClient::SendThread(void* pParam)
{
	SClient *pClient = (SClient*)pParam;//转换数据类型为SClient指针
	while(pClient->m_bConning)//连接状态
	{
        if(bSend)
        {
			//发送数据
			int val = send(pClient->m_socket, dataBuf, strlen(dataBuf),0);
			//处理返回错误
			if (val==SOCKET_ERROR)
			{
				int nErrCode=WSAGetLastError();
				if (nErrCode==WSAEWOULDBLOCK)//发送数据缓冲区不可用
				{
					continue;
				}else if (nErrCode==WSAENETDOWN||nErrCode==WSAETIMEDOUT||nErrCode==WSAECONNRESET)//客户端关闭了连接
				{
					pClient->m_bConning = false;	//连接断开
					break;
				}else {
					pClient->m_bConning = false;	//连接断开
					break;
				}
			}
			bSend=false;
			memset(dataBuf, 0, MAX_NUM_BUF);
		}

	}

	return 0;
}
