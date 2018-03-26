#include "TcpCli.h"

//client套接字
SOCKET sClient;
//发送标记位
bool toSend=false;
//与服务器的连接状态
bool bConnecting;
//发送数据线程
HANDLE SendHandle;
//接收数据线程
HANDLE RecvHandle;
//发送数据缓冲区
char bufSend[BUF_SIZE];



/**
 * 初始化全局变量
 */
void InitGlobal(void)
{
	sClient = INVALID_SOCKET;
	//初始化为为断开状态
	bConnecting = false;
    //初始化数据缓冲区
	memset(bufSend, 0, BUF_SIZE);
	//接收数据线程句柄
	RecvHandle = NULL;
	//发送数据线程句柄
	SendHandle = NULL;
}



//创建非阻塞套接字
bool  InitSocket(void)
{
	WSADATA		wsData;	//WSADATA变量
	int reVal = WSAStartup(MAKEWORD(2,2),&wsData);//初始化Windows Sockets Dll

	//创建套接字
	sClient = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET == sClient)
		return false;


	//设置套接字非阻塞模式
	unsigned long ul = 1;
	reVal = ioctlsocket(sClient, FIONBIO, (unsigned long*)&ul);
	if (reVal == SOCKET_ERROR)
		return false;

	return true;
}



//连接服务器
bool ConnectServer(void)
{
	int reVal;			//返回值
	sockaddr_in serAddr;//服务器地址
	//输入要连接的主机地址
    serAddr.sin_family=AF_INET;
    serAddr.sin_port=htons(SERVERPORT);
    serAddr.sin_addr.S_un.S_addr=inet_addr(SERVERIP);

    //设置尝试次数，超过一定次数则置为连接不成功状态
    int try_time=0;
	while(true)
	{
		//连接服务器
		reVal=connect(sClient, (struct sockaddr*)&serAddr, sizeof(serAddr));
		//检测已尝试次数
		if(try_time>LIMIT_FAIL)
            return false;
		//处理连接错误
		if(reVal==SOCKET_ERROR)
		{
			int nErrCode = WSAGetLastError();
			if( nErrCode==WSAEWOULDBLOCK || nErrCode==WSAEALREADY )    //连接还没有完成
			{
			    ++try_time;
				continue;
			}
			else if (nErrCode==WSAEISCONN)//连接已经完成
			{
				break;
			}
			else//其它原因，连接失败
			{
				return false;
			}
		}

		if(reVal==0)//连接成功
			break;
	}

	bConnecting = true;

	return true;
}



//创建发送和接收数据线程
bool	CreateThread(void)
{
	//创建接收数据的线程
	unsigned long ulThreadId;
	RecvHandle=CreateThread(NULL, 0, RecvThread, NULL, 0, &ulThreadId);
	if (RecvHandle==NULL)
		return false;

	//创建发送数据的线程
	SendHandle=CreateThread(NULL, 0, SendThread, NULL, 0, &ulThreadId);
	if (SendHandle==NULL)
		return false;

	return true;
}



//接收数据线程
DWORD __stdcall	RecvThread(void* pParam)
{
	int		reVal;
	//接收数据缓冲区
	char    bufRecv[BUF_SIZE];
    //当处于连接状态
	while(bConnecting)
	{
        memset(bufRecv, 0, BUF_SIZE);
        //接收数据
		reVal=recv(sClient, bufRecv, BUF_SIZE, 0);
		if (reVal==SOCKET_ERROR)
		{
			int nErrCode=WSAGetLastError();
			//接受数据缓冲区不可用
			if (nErrCode==WSAEWOULDBLOCK)
			{
			    //继续接收数据
				continue;
			}
			else
            {
				bConnecting=false;
				ShowMessage(ServerClose);
                bConnecting = false;
                toSend = false;
                //清空接收缓冲区
                memset(bufRecv, 0, BUF_SIZE);
                Exit(1);
				break;
			}
		}
		if(reVal > 0)
        {
            //显示数据
            cout<<"服务器向全体发送消息:"<<bufRecv<<endl;
        }
	}
	return 0;
}



//发送数据线程
DWORD __stdcall	SendThread(void* pParam)
{
    //当处于连接状态
	while(bConnecting)
	{
	    //如果有需要发送的数据
		if (toSend)
		{
			while(bConnecting)
            {
                int val=send(sClient, bufSend, BUF_SIZE,0);

                //处理返回错误
                if (val==SOCKET_ERROR)
                {
                    int nErrCode=WSAGetLastError();
                    if(nErrCode==WSAEWOULDBLOCK)		//发送缓冲区不可用
                    {
                        continue;						//继续循环
                    }else
                    {
                        return 0;
                    }
                }
                //缓冲区数据发送完毕，发送状态置为false
                toSend = false;
                break;
            }
		}
    }
	return 0;
}



//输入数据到写入缓冲区
void WriteBuffer(void)
{
    char cInput[BUF_SIZE];	//用户输入缓冲区
    while(bConnecting)			//连接状态
	{
		memset(cInput, 0, BUF_SIZE);
		cin.getline(cInput,BUF_SIZE);			        //输入表达式
		memcpy(bufSend, cInput, BUF_SIZE);
		toSend = true;
	}
}



//打印信息
void ShowMessage(int stateCode)
{
	switch(stateCode)
	{
    case InitSocketFail:
        cout<<"初始化套接字失败！"<<endl;
		cout<<"3秒钟后退出客户端..."<<endl;
		break;
    case ConnectServerFail:
        cout<<"连接服务器失败！"<<endl;
		cout<<"3秒钟后退出客户端..."<<endl;
		break;
    case ConnectServerSuccess:
        cout<<"成功连接至服务器！"<<endl;
        break;
    case CreateThreadFail:
        cout<<"收发数据线程创建失败！"<<endl;
        cout<<"3秒钟后退出客户端..."<<endl;
        break;
    case ServerClose:
        cout<<"检测到服务器已关闭"<<endl;
        cout<<"按回车退出客户端..."<<endl;
        break;
    default:
        cout<<"...."<<endl;
	}
}



//客户端退出
void Exit(int exittime)
{
    CloseHandle(RecvHandle);
	CloseHandle(SendHandle);
    memset(bufSend, 0, BUF_SIZE);
	closesocket(sClient);
	WSACleanup();
	Sleep(exittime);
}
