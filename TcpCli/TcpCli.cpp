#include "TcpCli.h"

//client�׽���
SOCKET sClient;
//���ͱ��λ
bool toSend=false;
//�������������״̬
bool bConnecting;
//���������߳�
HANDLE SendHandle;
//���������߳�
HANDLE RecvHandle;
//�������ݻ�����
char bufSend[BUF_SIZE];



/**
 * ��ʼ��ȫ�ֱ���
 */
void InitGlobal(void)
{
	sClient = INVALID_SOCKET;
	//��ʼ��ΪΪ�Ͽ�״̬
	bConnecting = false;
    //��ʼ�����ݻ�����
	memset(bufSend, 0, BUF_SIZE);
	//���������߳̾��
	RecvHandle = NULL;
	//���������߳̾��
	SendHandle = NULL;
}



//�����������׽���
bool  InitSocket(void)
{
	WSADATA		wsData;	//WSADATA����
	int reVal = WSAStartup(MAKEWORD(2,2),&wsData);//��ʼ��Windows Sockets Dll

	//�����׽���
	sClient = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET == sClient)
		return false;


	//�����׽��ַ�����ģʽ
	unsigned long ul = 1;
	reVal = ioctlsocket(sClient, FIONBIO, (unsigned long*)&ul);
	if (reVal == SOCKET_ERROR)
		return false;

	return true;
}



//���ӷ�����
bool ConnectServer(void)
{
	int reVal;			//����ֵ
	sockaddr_in serAddr;//��������ַ
	//����Ҫ���ӵ�������ַ
    serAddr.sin_family=AF_INET;
    serAddr.sin_port=htons(SERVERPORT);
    serAddr.sin_addr.S_un.S_addr=inet_addr(SERVERIP);

    //���ó��Դ���������һ����������Ϊ���Ӳ��ɹ�״̬
    int try_time=0;
	while(true)
	{
		//���ӷ�����
		reVal=connect(sClient, (struct sockaddr*)&serAddr, sizeof(serAddr));
		//����ѳ��Դ���
		if(try_time>LIMIT_FAIL)
            return false;
		//�������Ӵ���
		if(reVal==SOCKET_ERROR)
		{
			int nErrCode = WSAGetLastError();
			if( nErrCode==WSAEWOULDBLOCK || nErrCode==WSAEALREADY )    //���ӻ�û�����
			{
			    ++try_time;
				continue;
			}
			else if (nErrCode==WSAEISCONN)//�����Ѿ����
			{
				break;
			}
			else//����ԭ������ʧ��
			{
				return false;
			}
		}

		if(reVal==0)//���ӳɹ�
			break;
	}

	bConnecting = true;

	return true;
}



//�������ͺͽ��������߳�
bool	CreateThread(void)
{
	//�����������ݵ��߳�
	unsigned long ulThreadId;
	RecvHandle=CreateThread(NULL, 0, RecvThread, NULL, 0, &ulThreadId);
	if (RecvHandle==NULL)
		return false;

	//�����������ݵ��߳�
	SendHandle=CreateThread(NULL, 0, SendThread, NULL, 0, &ulThreadId);
	if (SendHandle==NULL)
		return false;

	return true;
}



//���������߳�
DWORD __stdcall	RecvThread(void* pParam)
{
	int		reVal;
	//�������ݻ�����
	char    bufRecv[BUF_SIZE];
    //����������״̬
	while(bConnecting)
	{
        memset(bufRecv, 0, BUF_SIZE);
        //��������
		reVal=recv(sClient, bufRecv, BUF_SIZE, 0);
		if (reVal==SOCKET_ERROR)
		{
			int nErrCode=WSAGetLastError();
			//�������ݻ�����������
			if (nErrCode==WSAEWOULDBLOCK)
			{
			    //������������
				continue;
			}
			else
            {
				bConnecting=false;
				ShowMessage(ServerClose);
                bConnecting = false;
                toSend = false;
                //��ս��ջ�����
                memset(bufRecv, 0, BUF_SIZE);
                Exit(1);
				break;
			}
		}
		if(reVal > 0)
        {
            //��ʾ����
            cout<<"��������ȫ�巢����Ϣ:"<<bufRecv<<endl;
        }
	}
	return 0;
}



//���������߳�
DWORD __stdcall	SendThread(void* pParam)
{
    //����������״̬
	while(bConnecting)
	{
	    //�������Ҫ���͵�����
		if (toSend)
		{
			while(bConnecting)
            {
                int val=send(sClient, bufSend, BUF_SIZE,0);

                //�����ش���
                if (val==SOCKET_ERROR)
                {
                    int nErrCode=WSAGetLastError();
                    if(nErrCode==WSAEWOULDBLOCK)		//���ͻ�����������
                    {
                        continue;						//����ѭ��
                    }else
                    {
                        return 0;
                    }
                }
                //���������ݷ�����ϣ�����״̬��Ϊfalse
                toSend = false;
                break;
            }
		}
    }
	return 0;
}



//�������ݵ�д�뻺����
void WriteBuffer(void)
{
    char cInput[BUF_SIZE];	//�û����뻺����
    while(bConnecting)			//����״̬
	{
		memset(cInput, 0, BUF_SIZE);
		cin.getline(cInput,BUF_SIZE);			        //������ʽ
		memcpy(bufSend, cInput, BUF_SIZE);
		toSend = true;
	}
}



//��ӡ��Ϣ
void ShowMessage(int stateCode)
{
	switch(stateCode)
	{
    case InitSocketFail:
        cout<<"��ʼ���׽���ʧ�ܣ�"<<endl;
		cout<<"3���Ӻ��˳��ͻ���..."<<endl;
		break;
    case ConnectServerFail:
        cout<<"���ӷ�����ʧ�ܣ�"<<endl;
		cout<<"3���Ӻ��˳��ͻ���..."<<endl;
		break;
    case ConnectServerSuccess:
        cout<<"�ɹ���������������"<<endl;
        break;
    case CreateThreadFail:
        cout<<"�շ������̴߳���ʧ�ܣ�"<<endl;
        cout<<"3���Ӻ��˳��ͻ���..."<<endl;
        break;
    case ServerClose:
        cout<<"��⵽�������ѹر�"<<endl;
        cout<<"���س��˳��ͻ���..."<<endl;
        break;
    default:
        cout<<"...."<<endl;
	}
}



//�ͻ����˳�
void Exit(int exittime)
{
    CloseHandle(RecvHandle);
	CloseHandle(SendHandle);
    memset(bufSend, 0, BUF_SIZE);
	closesocket(sClient);
	WSACleanup();
	Sleep(exittime);
}
