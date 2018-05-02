#ifndef SCLIENT_H_INCLUDED
#define SCLIENT_H_INCLUDED

#include <windows.h>
#include <iostream>
using namespace std;

//����������󳤶�
#define	MAX_NUM_BUF	100

class SClient
{
public:
	SClient(const SOCKET sClient,const sockaddr_in &addrClient);
	~SClient();
    //�������ͺͽ��������߳�
	bool StartRuning(void);
	//�Ƿ����Ӵ���
	bool IsConning(void)
	{
        return m_bConning;
    }
    void Setm_bSend(bool Bool)
    {
        m_bSend=Bool;
    }
    //�Ͽ���ͻ��˵�����
	void DisConning(void)
	{
        m_bConning = false;
    }
    //��ȡ��ַ��Ϣ
    sockaddr_in Getm_addr(void)
    {
        return m_addr;
    }
    //д�뻺����
    void SetBufSend(char *str)
    {
        memcpy(BufSend, str, MAX_NUM_BUF);
    }
    //���տͻ�������
	static DWORD __stdcall RecvThread(void* pParam);
	//��ͻ��˷�������
	static DWORD __stdcall SendThread(void* pParam);

private:
	SOCKET m_socket;			//�׽���
	sockaddr_in	m_addr;			//��ַ
	char BufSend[MAX_NUM_BUF];	//����
	HANDLE m_SendHandle;		//���������߳̾��
	HANDLE m_RecvHandle;		//���������߳̾��
	bool m_bConning;			//�ͻ�������״̬
	bool m_bSend;
};

#endif // SCLIENT_H_INCLUDED
