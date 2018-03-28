#ifndef CLASS_H_INCLUDED
#define CLASS_H_INCLUDED

#include <windows.h>
#include <iostream>
using namespace std;

//����������󳤶�
#define	MAX_NUM_BUF	100

//д������
extern char	dataBuf[MAX_NUM_BUF];
//���ͱ��λ
extern bool bSend;

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
};

#endif // CLASS_H_INCLUDED
