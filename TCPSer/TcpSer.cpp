#include "TcpSer.h"
#include "Class.h"


/**
 * ȫ�ֱ���
 */
char	dataBuf[MAX_NUM_BUF];				//д������
bool	bConning;							//��ͻ��˵�����״̬
bool    bSend;                              //���ͱ��λ
bool    clientConn;                         //���ӿͻ��˱��
SOCKET	sServer;							//�����������׽���
HANDLE	AccHandle;						//���ݴ����߳̾��
HANDLE	CleanHandle;						//���ݽ����߳�
ClIENTVECTOR clientvector;                  //�洢���׽���


/**
 * ��ʼ��ȫ�ֱ���
 */
void InitGlobal(void)
{
	memset(dataBuf, 0, MAX_NUM_BUF);
	bSend = false;
	clientConn = false;
	bConning = false;									    //������Ϊû������״̬
	AccHandle = NULL;									//����ΪNULL
	CleanHandle = NULL;
	sServer = INVALID_SOCKET;								//����Ϊ��Ч���׽���
	clientvector.clear();									//�������
}

/**
 *  ��ʼ��SOCKET
 */
bool initSocket(void)
{
	//����ֵ
	int reVal;

	//��ʼ��Windows Sockets DLL
	WSADATA  wsData;
	reVal = WSAStartup(MAKEWORD(2,2),&wsData);

	//�����׽���
	sServer = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET== sServer)
		return false;

	//�����׽��ַ�����ģʽ
	unsigned long ul = 1;
	reVal = ioctlsocket(sServer, FIONBIO, (unsigned long*)&ul);
	if (SOCKET_ERROR == reVal)
		return false;

	//���׽���
	sockaddr_in serAddr;
	serAddr.sin_family=AF_INET;
	serAddr.sin_port=htons(SERVERPORT);
	serAddr.sin_addr.S_un.S_addr=inet_addr(SERVERIP);
	reVal = bind(sServer, (sockaddr*)&serAddr, sizeof(serAddr));
	if(SOCKET_ERROR == reVal )
		return false;

	//����
	reVal = listen(sServer, CONN_NUM);
	if(SOCKET_ERROR == reVal)
		return false;

	return true;
}


/**
 * ����������Դ�ͽ��ܿͻ��������߳�
 */
bool createThread(void)
{
    bConning = true;//���÷�����Ϊ����״̬

	//�����ͷ���Դ�߳�
	unsigned long ulThreadId;
	//�������տͻ��������߳�
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
	//�������������߳�
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
 * ���ܿͻ�������
 */
DWORD __stdcall acceptThread(void* pParam)
{
    SOCKET  sAccept;							                        //���ܿͻ������ӵ��׽���
	sockaddr_in addrClient;						                        //�ͻ���SOCKET��ַ

	while(bConning)						                                //��������״̬
	{
		memset(&addrClient, 0, sizeof(sockaddr_in));					//��ʼ��
		int	lenClient=sizeof(sockaddr_in);				        	//��ַ����
		sAccept=accept(sServer, (sockaddr*)&addrClient, &lenClient);	//���ܿͻ�����
		if(sAccept==INVALID_SOCKET)
		{
			int nErrCode=WSAGetLastError();
			if(nErrCode==WSAEWOULDBLOCK)	                            //�޷��������һ�����赲���׽��ֲ���
			{
				Sleep(1000);
				continue;                                               //�����ȴ�
			}
			else
            {
				return 0;                                               //�߳��˳�
			}

		}
		else//���ܿͻ��˵�����
		{
		    clientConn = true;          //�Ѿ������Ͽͻ���
		    SClient *pClient = new SClient(sAccept, addrClient);
            //��ʾ�ͻ��˵�IP�Ͷ˿�
            char *pClientIP = inet_ntoa(addrClient.sin_addr);
            cout<<"IP: "<<pClientIP<<"\t�ͻ���������"<<endl;
			clientvector.push_back(pClient);            //��������

            pClient->StartRuning();
		}
	}
	return 0;//�߳��˳�
}

/**
 * ������Դ�߳�
 */
DWORD __stdcall cleanThread(void* pParam)
 {
    while (bConning)                  //��������������
	{
		//�����Ѿ��Ͽ������ӿͻ����ڴ�ռ�
		ClIENTVECTOR::iterator iter = clientvector.begin();
		for (iter; iter != clientvector.end();)
		{
			SClient *pClient = (SClient*)*iter;
			if (!pClient->IsConning())			//�ͻ����߳��Ѿ��˳�
			{
			    char *pClientIP=inet_ntoa(pClient->Getm_addr().sin_addr);
			    cout<<"IP��"<<pClientIP<<"\t�ͻ����ѶϿ�����"<<endl;
				clientvector.erase(iter);	//ɾ���ڵ�
				delete pClient;				//�ͷ��ڴ�
				pClient = NULL;
			}else{
				iter++;						//ָ������
			}
		}
		if(clientvector.size() == 0)
        {
            clientConn = false;
        }

		Sleep(1000);
	}


	//������ֹͣ����
	if (!bConning)
	{
		ClIENTVECTOR::iterator iter = clientvector.begin();
		for (iter; iter != clientvector.end();)
		{
			SClient *pClient = (SClient*)*iter;
			//����ͻ��˵����ӻ����ڣ���Ͽ����ӣ��߳��˳�
			if (pClient->IsConning())
			{
				pClient->DisConning();
			}
			++iter;
		}
		//�����ӿͻ����߳�ʱ�䣬ʹ���Զ��˳�
		Sleep(TIMEFOR_THREAD_HELP);
	}

	clientvector.clear();		//�������
	clientConn = false;

	return 0;
 }

/**
 * ��������
 */
 void SendBuffer(void)
 {
    char sendBuf[MAX_NUM_BUF];

    while(bConning)
    {
        memset(sendBuf, 0, MAX_NUM_BUF);	//��շ��ͻ�����
        cin.getline(sendBuf,MAX_NUM_BUF);
        memcpy(dataBuf, sendBuf, MAX_NUM_BUF);
        bSend = true;
    }
 }

/**
 *  �ͷ���Դ
 */
void  Exit(void)
{
	closesocket(sServer);					//�ر�SOCKET
	WSACleanup();							//ж��Windows Sockets DLL
}


/**
 * ��ʾ�����������ɹ���ʧ����Ϣ
 */
void  showServerStartMsg(bool bSuc)
{
	if (bSuc)
	{
		cout<<"�����������ɹ���"<<endl;
		cout<<"�ȴ��ͻ�������.."<<endl;
	}else{
		cout<<"����������ʧ�ܣ�"<<endl;
	}

}

/**
 * ��ʾ�������˳���Ϣ
 */
void  showServerExitMsg(void)
{
	cout << "�������˳�.." << endl;
}
