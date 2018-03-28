#include "TcpSer.h"
#include "Class.h"


/**
 * 全局变量
 */
char	dataBuf[MAX_NUM_BUF];				//写缓冲区
bool	bConning;							//与客户端的连接状态
bool    bSend;                              //发送标记位
bool    clientConn;                         //连接客户端标记
SOCKET	sServer;							//服务器监听套接字
HANDLE	AccHandle;						//数据处理线程句柄
HANDLE	CleanHandle;						//数据接收线程
ClIENTVECTOR clientvector;                  //存储子套接字


/**
 * 初始化全局变量
 */
void InitGlobal(void)
{
	memset(dataBuf, 0, MAX_NUM_BUF);
	bSend = false;
	clientConn = false;
	bConning = false;									    //服务器为没有运行状态
	AccHandle = NULL;									//设置为NULL
	CleanHandle = NULL;
	sServer = INVALID_SOCKET;								//设置为无效的套接字
	clientvector.clear();									//清空向量
}

/**
 *  初始化SOCKET
 */
bool initSocket(void)
{
	//返回值
	int reVal;

	//初始化Windows Sockets DLL
	WSADATA  wsData;
	reVal = WSAStartup(MAKEWORD(2,2),&wsData);

	//创建套接字
	sServer = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET== sServer)
		return false;

	//设置套接字非阻塞模式
	unsigned long ul = 1;
	reVal = ioctlsocket(sServer, FIONBIO, (unsigned long*)&ul);
	if (SOCKET_ERROR == reVal)
		return false;

	//绑定套接字
	sockaddr_in serAddr;
	serAddr.sin_family=AF_INET;
	serAddr.sin_port=htons(SERVERPORT);
	serAddr.sin_addr.S_un.S_addr=inet_addr(SERVERIP);
	reVal = bind(sServer, (sockaddr*)&serAddr, sizeof(serAddr));
	if(SOCKET_ERROR == reVal )
		return false;

	//监听
	reVal = listen(sServer, CONN_NUM);
	if(SOCKET_ERROR == reVal)
		return false;

	return true;
}


/**
 * 产生清理资源和接受客户端连接线程
 */
bool createThread(void)
{
    bConning = true;//设置服务器为运行状态

	//创建释放资源线程
	unsigned long ulThreadId;
	//创建接收客户端请求线程
	AccHandle=CreateThread(NULL, 0, acceptThread, NULL, 0, &ulThreadId);
	if(AccHandle==NULL)
	{
		bConning = false;
		return false;
	}
	else
    {
		CloseHandle(AccHandle);
	}
	//创建接收数据线程
	CleanHandle=CreateThread(NULL, 0, cleanThread, NULL, 0, &ulThreadId);
	if(CleanHandle==NULL)
	{
		return false;
	}
	else
    {
		CloseHandle(CleanHandle);
	}
	return true;
}


/**
 * 接受客户端连接
 */
DWORD __stdcall acceptThread(void* pParam)
{
    SOCKET  sAccept;							                        //接受客户端连接的套接字
	sockaddr_in addrClient;						                        //客户端SOCKET地址

	while(bConning)						                                //服务器的状态
	{
		memset(&addrClient, 0, sizeof(sockaddr_in));					//初始化
		int	lenClient=sizeof(sockaddr_in);				        	//地址长度
		sAccept=accept(sServer, (sockaddr*)&addrClient, &lenClient);	//接受客户请求
		if(sAccept==INVALID_SOCKET)
		{
			int nErrCode=WSAGetLastError();
			if(nErrCode==WSAEWOULDBLOCK)	                            //无法立即完成一个非阻挡性套接字操作
			{
				Sleep(1000);
				continue;                                               //继续等待
			}
			else
            {
				return 0;                                               //线程退出
			}

		}
		else//接受客户端的请求
		{
		    clientConn = true;          //已经连接上客户端
		    SClient *pClient = new SClient(sAccept, addrClient);
            //显示客户端的IP和端口
            char *pClientIP = inet_ntoa(addrClient.sin_addr);
            cout<<"IP: "<<pClientIP<<"\t客户端已连接"<<endl;
			clientvector.push_back(pClient);            //加入容器

            pClient->StartRuning();
		}
	}
	return 0;//线程退出
}

/**
 * 清理资源线程
 */
DWORD __stdcall cleanThread(void* pParam)
 {
    while (bConning)                  //服务器正在运行
	{
		//清理已经断开的连接客户端内存空间
		ClIENTVECTOR::iterator iter = clientvector.begin();
		for (iter; iter != clientvector.end();)
		{
			SClient *pClient = (SClient*)*iter;
			if (!pClient->IsConning())			//客户端线程已经退出
			{
			    char *pClientIP=inet_ntoa(pClient->Getm_addr().sin_addr);
			    cout<<"IP："<<pClientIP<<"\t客户端已断开连接"<<endl;
				clientvector.erase(iter);	//删除节点
				delete pClient;				//释放内存
				pClient = NULL;
			}else{
				iter++;						//指针下移
			}
		}
		if(clientvector.size() == 0)
        {
            clientConn = false;
        }

		Sleep(1000);
	}


	//服务器停止工作
	if (!bConning)
	{
		ClIENTVECTOR::iterator iter = clientvector.begin();
		for (iter; iter != clientvector.end();)
		{
			SClient *pClient = (SClient*)*iter;
			//如果客户端的连接还存在，则断开连接，线程退出
			if (pClient->IsConning())
			{
				pClient->DisConning();
			}
			++iter;
		}
		//给连接客户端线程时间，使其自动退出
		Sleep(TIMEFOR_THREAD_HELP);
	}

	clientvector.clear();		//清空链表
	clientConn = false;

	return 0;
 }

/**
 * 处理数据
 */
 void SendBuffer(void)
 {
    char sendBuf[MAX_NUM_BUF];

    while(bConning)
    {
        memset(sendBuf, 0, MAX_NUM_BUF);	//清空发送缓冲区
        cin.getline(sendBuf,MAX_NUM_BUF);
        memcpy(dataBuf, sendBuf, MAX_NUM_BUF);
        bSend = true;
    }
 }

/**
 *  释放资源
 */
void  Exit(void)
{
	closesocket(sServer);					//关闭SOCKET
	WSACleanup();							//卸载Windows Sockets DLL
}


/**
 * 显示启动服务器成功与失败消息
 */
void  showServerStartMsg(bool bSuc)
{
	if (bSuc)
	{
		cout<<"服务器创建成功！"<<endl;
		cout<<"等待客户端连接.."<<endl;
	}else{
		cout<<"服务器创建失败！"<<endl;
	}

}

/**
 * 显示服务器退出消息
 */
void  showServerExitMsg(void)
{
	cout << "服务器退出.." << endl;
}
