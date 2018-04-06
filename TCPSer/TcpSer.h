#ifndef TCPSER_H_INCLUDED
#define TCPSER_H_INCLUDED
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <process.h>
#include "SClient.h"

using namespace std;
#pragma comment(lib, "ws2_32.lib")			//��̬�⺯��

#define SERVERIP "127.0.0.1"//������TCP��IP��ַ
#define SERVERPORT 6666		 //������TCP�˿�
#define CONN_NUM 10              //���ӿͻ�������

#define TIMEFOR_THREAD_HELP			1500	//������Դ�߳��˳�ʱ��
#define TIMEFOR_THREAD_EXIT			5000	//���߳�˯��ʱ��

typedef vector<SClient*> ClIENTVECTOR;		//��������

//��ʼ��ȫ�ֱ���
void InitGlobal(void);
//��ʼ���������׽���
bool initSocket(void);
//�˳�������
void Exit(void);
//��ȡ������Ϣ
void SendBuffer(void);
//�������ݸ����ͻ���
void ToAllCliBuf(char* str, char* addr);
//��ʾ������������Ϣ
void showServerStartMsg(bool bSuc);
//��ʾ�˳���Ϣ
void showServerExitMsg(void);
//�����̣߳����м������������
bool createThread(void);
//���ܿͻ��������߳�ִ�к���
DWORD __stdcall acceptThread(void* pParam);
//�����ѶϿ����ӿͻ��˵��߳�ִ�к���
DWORD __stdcall cleanThread(void* pParam);

#endif // TCPSER_H_INCLUDED
