#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h> 

using namespace std;

void InitListen();
void StartEpoll();
void BoardCast(char* msg, long int nLen);
void removeFd(int fd);
 
void InitListen()
{
	
}

void StartEpoll()
{

}

void BoardCast(char *msg, long int nLen)
{

}

void removeFd(int fd)
{

}


int main()
{
    InitListen();
    StartEpoll();
    return 0;
}
