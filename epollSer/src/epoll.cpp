#include "global.h"

//监听套接字
int epSer;
//epoll专用的描述符
int epFd;
//记录已连接客户端套接字的容器
vector<int> clisockets;
//发送缓冲区
char sendBuf[BUF_SIZE];
//接收缓冲区
char recvBuf[BUF_SIZE];

//初始化全局变量
void InitGlobal()
{
	epSer=INVALID_SOCKET;
	epFd=INVALID_SOCKET;
	clisockets.clear();
	memset(sendBuf, 0, BUF_SIZE);
	memset(recvBuf, 0, BUF_SIZE);
}

//初始化套接字
bool InitSocket()
{
	int reVal;
	//创建服务器监听套接字
	epSer=socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET==epSer)
	{
		return false;
	}
	//设置服务器监听套接字地址
	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr=inet_addr(SERVERIP);
	seraddr.sin_port=htons(SERVERPORT);
	//将监听套接字和地址绑定
	reVal=bind(epSer, (sockaddr*)&seraddr, sizeof(sockaddr));
	if(SOCKET_ERROR==reVal)
	{
		return false;
	}
	//对监听套接字进行监听
	reVal=listen(epSer, CONN_NUM);
	if(SOCKET_ERROR==reVal)
	{
		return false;
	}

	return true;
}


bool StartEpoll()
{
	int reVal;
	//创建epoll专用的文件描述符
	epFd=epoll_create(EPOLL_SIZE);

	epoll_event EpEvnt;
	EpEvnt.data.fd=epSer;
	EpEvnt.events=EPOLLIN|EPOLLET|EPOLLOUT;
	//对epoll事件进行注册
	reVal=epoll_ctl(epFd ,EPOLL_CTL_ADD, epSer ,&EpEvnt);
	if(-1==reVal)
	{
		return false;
	}
	return true;
}

void Epoll()
{
	epoll_event Ev[EPOLL_SIZE+1];
	while(true)
	{
		int nfds=epoll_wait(epFd, Ev, 10, 1);
		for(int i=0;i<nfds;i++)
		{
			//有新的客户端发起连接请求
			if(Ev[i].data.fd==epSer)
			{
				AcceptConn();
			}
			//有客户端发送数据
			else if(Ev[i].events&EPOLLIN)
			{
				//从触发事件的套接字中读取数据，写入到recvBuf
				int msglen=GetMessage(Ev[i].data.fd);
				//将sendBuf内容发送到全体已连接套接字
				Boardcast(msglen);
			}
			//有客户端断开连接
			else if(Ev[i].events&EPOLLHUP)
			{
				RemoveConn(Ev[i].data.fd);
			}
		}
	}
}

void AcceptConn()
{
	//获取发送请求的客户端信息
	sockaddr_in cliaddr;
	unsigned int len=sizeof(cliaddr);
	int clisocket=accept(epSer, (sockaddr*)&cliaddr, &len);

	if(INVALID_SOCKET==clisocket)
	{
		cout<<"accept Error!";
	}

	epoll_event clievent;
	clievent.data.fd=clisocket;
	clievent.events=EPOLLIN|EPOLLOUT|EPOLLHUP;
	//将与客户端建立连接的套接字，加入到epoll监听队列中
	epoll_ctl(epFd, EPOLL_CTL_ADD, clisocket, &clievent);
	//加入到记录已连接套接字的容器中
	clisockets.push_back(clisocket);
}

int GetMessage(int clifd)
{
	int msglen=recv(clifd, recvBuf, BUF_SIZE, 0);
	cout<<"客户端:"<<clifd<<"  发来数据:"<<recvBuf<<endl;
	return msglen;
}

void Boardcast(char* buf, int len)
{
	strcpy(sendBuf, recvBuf);
	vector<int>::iterator item;
	//迭代器遍历记录所有已连接客户端的容器，并发送数据
	for(item=clisockets.begin();item!=clisockets.end();item++)
	{
		send(*item, sendBuf, BUF_SIZE, 0);
	}
	cout<<"已向全体客户端发送数据"<<sendBuf<<endl;
}

void RemoveConn(int clifd)
{
	epoll_ctl(epFd, EPOLL_CTL_DEL, clifd, NULL);
	vector<int>::iterator item;
	//遍历容器，找出已断开连接的客户端，将之从epoll监听队列中删除
	for(item=clisockets.begin(); item!=clisockets.end(); item++)
	{
		if(*item==clifd)
		{
			clisockets.erase(item);
		}
	}
}

void Exit()
{
	close(epSer);
}


