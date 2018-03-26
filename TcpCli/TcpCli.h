#ifndef TCPCLI_H_INCLUDED
#define TCPCLI_H_INCLUDED

#include <iostream>
#include <windows.h>
#include <process.h>

#pragma comment(lib, "WS2_32.lib")
using namespace std;

//������IP
#define	SERVERIP "127.0.0.1"
//������TCP�˿�
#define	SERVERPORT 6666


//����������󳤶�
#define	BUF_SIZE 100
//�����������ʧ�ܴ���
#define LIMIT_FAIL 10
#define EXITTIME 3000

#define InitSocketFail 1
#define ConnectServerSuccess 2
#define ConnectServerFail 3
#define CreateThreadSuccess 4
#define CreateThreadFail 5
#define ServerClose 6

//��ʼ��ȫ�ֱ���
void InitGlobal(void);
//�������׽���
bool InitSocket(void);
//���ӷ�����
bool ConnectServer(void);
//��ӡ��Ϣ
void ShowMessage(int stateCode);
//�����շ����ݵ��߳�
bool CreateThread(void);
//���������̵߳�ִ�к���
DWORD __stdcall	RecvThread(void*);
//���������̵߳�ִ�к���
DWORD __stdcall	SendThread(void*);

void WriteBuffer();
//�˳��ͻ���
void Exit(int exittime);

#endif // TCPCLI_H_INCLUDED
