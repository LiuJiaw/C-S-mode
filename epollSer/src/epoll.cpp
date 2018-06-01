#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "global.h"

using namespace std;


void InitGlobal()
{
	epSer=INVALID_SOCKET;
}

bool InitSocket()
{
	int reVal;

	epSer=socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET==epSer)
	{
		return false;
	}

	sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr=SERVERIP;
	seraddr.sin_port=htons(SERVERPORT);

	reVal=bind(epSer, (sockaddr_in*)&seraddr, sizeof(sockaddr_in));
	if(SOCKET_ERROR==reVal)
	{
		return false;
	}

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

	epFd=epoll_create(EPOLL_SIZE);

	epoll_event EpEvnt;
	EpEvnt.data.fd=epSer;
	EpEvnt.events=EPOLLIN|EPOLLET|EPOLLOUT;

	reVal=epoll_ctl(epFd ,EPOLL_CTL_ADD, epSer ,&EpEvnt);
	if(-1==reVal)
	{
		return false;
	}
	return true;
}

void Epoll()
{
	while(true)
	{

	}
}


void Exit()
{
	close(epSer);
}


