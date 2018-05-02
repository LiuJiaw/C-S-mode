#ifndef SCLIENT_H_INCLUDED
#define SCLIENT_H_INCLUDED

#include <windows.h>
#include <iostream>
using namespace std;

//缓冲区的最大长度
#define	MAX_NUM_BUF	100

class SClient
{
public:
	SClient(const SOCKET sClient,const sockaddr_in &addrClient);
	~SClient();
    //创建发送和接收数据线程
	bool StartRuning(void);
	//是否连接存在
	bool IsConning(void)
	{
        return m_bConning;
    }
    void Setm_bSend(bool Bool)
    {
        m_bSend=Bool;
    }
    //断开与客户端的连接
	void DisConning(void)
	{
        m_bConning = false;
    }
    //获取地址信息
    sockaddr_in Getm_addr(void)
    {
        return m_addr;
    }
    //写入缓冲区
    void SetBufSend(char *str)
    {
        memcpy(BufSend, str, MAX_NUM_BUF);
    }
    //接收客户端数据
	static DWORD __stdcall RecvThread(void* pParam);
	//向客户端发送数据
	static DWORD __stdcall SendThread(void* pParam);

private:
	SOCKET m_socket;			//套接字
	sockaddr_in	m_addr;			//地址
	char BufSend[MAX_NUM_BUF];	//数据
	HANDLE m_SendHandle;		//发送数据线程句柄
	HANDLE m_RecvHandle;		//接收数据线程句柄
	bool m_bConning;			//客户端连接状态
	bool m_bSend;
};

#endif // SCLIENT_H_INCLUDED
