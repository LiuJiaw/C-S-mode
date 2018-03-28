#include <process.h>
#include <stdio.h>
#include "Class.h"
#include "TcpSer.h"


//���캯��
SClient::SClient(const SOCKET sClient, const sockaddr_in &addrClient)
{
	//��ʼ������
	m_RecvHandle=NULL;
	m_SendHandle=NULL;
	m_socket=sClient;
	m_addr=addrClient;
	m_bConning=false;
	memset(BufSend, 0, MAX_NUM_BUF);
}
//��������
SClient::~SClient()
{
	closesocket(m_socket);			//�ر��׽���
	m_socket=INVALID_SOCKET;		//�׽�����Ч
}

//�������ͺͽ��������߳�
bool SClient::StartRuning(void)
{
	m_bConning=true;//��������״̬

	//�������������߳�
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

	//�������տͻ������ݵ��߳�
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


//���տͻ�������
DWORD  SClient::RecvThread(void* pParam)
{
	SClient *pClient=(SClient*)pParam;	//�ͻ��˶���ָ��
	int		reVal;							//����ֵ
	char	temp[MAX_NUM_BUF];				//��ʱ����


	while(pClient->m_bConning)				//����״̬
	{
	    memset(temp, 0, MAX_NUM_BUF);
		reVal = recv(pClient->m_socket, temp, MAX_NUM_BUF, 0);	//��������

		//������󷵻�ֵ
		if (reVal==SOCKET_ERROR)
		{
			int nErrCode=WSAGetLastError();

			if (nErrCode==WSAEWOULDBLOCK)	//�������ݻ�����������
			{
				continue;						//����ѭ��
			}
			//�ͻ��˹ر�������
			else if (nErrCode==WSAENETDOWN ||nErrCode==WSAETIMEDOUT ||nErrCode==WSAECONNRESET)
			{
				break;							//�߳��˳�
			}
		}

		//�ͻ��˹ر�������
		if ( reVal == 0)
		{
			break;
		}

		//�յ�����
		if (reVal > 0)
		{
		    char *pClientIP = inet_ntoa(pClient->m_addr.sin_addr);
            u_short  clientPort = ntohs(pClient->m_addr.sin_port);
			cout<<"IP: "<<pClientIP<<"\t��������������ݣ�"<<temp<<endl;      //�����ʾ����
		}
	}
	pClient->m_bConning = false;			//��ͻ��˵����ӶϿ�
	return 0;								//�߳��˳�
}

//��ͻ��˷�������
DWORD SClient::SendThread(void* pParam)
{
	SClient *pClient = (SClient*)pParam;//ת����������ΪSClientָ��
	while(pClient->m_bConning)//����״̬
	{
        if(bSend)
        {
			//��������
			int val = send(pClient->m_socket, dataBuf, strlen(dataBuf),0);
			//�����ش���
			if (val==SOCKET_ERROR)
			{
				int nErrCode=WSAGetLastError();
				if (nErrCode==WSAEWOULDBLOCK)//�������ݻ�����������
				{
					continue;
				}else if (nErrCode==WSAENETDOWN||nErrCode==WSAETIMEDOUT||nErrCode==WSAECONNRESET)//�ͻ��˹ر�������
				{
					pClient->m_bConning = false;	//���ӶϿ�
					break;
				}else {
					pClient->m_bConning = false;	//���ӶϿ�
					break;
				}
			}
			bSend=false;
			memset(dataBuf, 0, MAX_NUM_BUF);
		}

	}

	return 0;
}
