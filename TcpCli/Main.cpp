#include <iostream>
#include "TcpCli.h"

using namespace std;

int main()
{
    //��ʼ��ȫ�ֱ���
    InitGlobal();
    //��ʼ���׽���
	int initstate=InitSocket();
	if(!initstate)
    {
        ShowMessage(InitSocketFail);
        Exit(EXITTIME);
        return -1;
    }


	//�������ӷ�������ConnectServer����true��ʾ���ӳɹ�
	int connectstate=ConnectServer();
	if(connectstate)
		ShowMessage(ConnectServerSuccess);
	else
    {
		ShowMessage(ConnectServerFail);
        Exit(EXITTIME);
        return -1;
	}

	//�����շ������̣߳�CreateThread����true��ʾ�����ɹ�
	int Createstate=CreateThread();
	if(!Createstate)
    {
        ShowMessage(CreateThreadFail);
        Exit(EXITTIME);
        return -1;
    }

	//�û��������ݺ���ʾ���
	WriteBuffer();

	return 0;
}
