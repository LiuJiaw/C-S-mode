#ifndef TCPSER_H_INCLUDED
#define TCPSER_H_INCLUDED
#include <iostream>
#include <windows.h>
#include <winbase.h>
#include <vector>
#include <map>
#include <string>
#include <process.h>
#include "Class.h"

using namespace std;
#pragma comment(lib, "ws2_32.lib")			//��̬�⺯��

#define SERVERIP "127.0.0.1"//������TCP��IP��ַ
#define SERVERPORT 6666		 //������TCP�˿�
#define CONN_NUM 10              //���ӿͻ�������

#define TIMEFOR_THREAD_HELP			1500	//������Դ�߳��˳�ʱ��
#define TIMEFOR_THREAD_EXIT			5000	//���߳�˯��ʱ��

typedef vector<CClient*> ClIENTVECTOR;		//��������
typedef vector<string> SVECTOR;             //�����ַ�


/**
 * ȫ�ֱ���
 */
extern char	dataBuf[MAX_NUM_BUF];				//д������
extern bool	bConning;							//��ͻ��˵�����״̬
extern bool bSend;                              //���ͱ��λ
extern bool clientConn;                         //���ӿͻ��˱��
extern SOCKET sServer;							//�����������׽���
extern HANDLE AccHandle;						//���ݴ����߳̾��
extern HANDLE CleanHandle;						//���ݽ����߳�

/**
 *��������
 */
bool initSever(void);                       //��ʼ��
void InitGlobal(void);
bool initSocket(void);						//��ʼ���������׽���
void exitServer(void);						//�ͷ���Դ
bool startService(void);					//����������
void inputAndOutput(void);                  //��������
void showServerStartMsg(bool bSuc);         //��ʾ������Ϣ
void showServerExitMsg(void);               //��ʾ�˳���Ϣ
void handleData(char* str);                 //���ݴ���
void showTipMsg(int bFirstInput);          //��ʾ������ʾ��Ϣ
bool createCleanAndAcceptThread(void);      //������غ���
DWORD __stdcall acceptThread(void* pParam); //�����ͻ��������߳�
DWORD __stdcall cleanThread(void* pParam);

#endif // TCPSER_H_INCLUDED
