#include "TcpSer.h"


int main()
{
    //��ʼ��ȫ�ֱ���
    InitGlobal();
	//��ʼ���׽���
	if (!initSocket())
	{
		exitServer();
    	return -1;
	}

	if(createCleanAndAcceptThread())	//���ܿͻ���������߳�
    {
        showServerStartMsg(true);		//�����̳߳ɹ���Ϣ
    }
    else
    {
        showServerStartMsg(false);
		exitServer();
    	return -1;
    }

	 //��������
    inputAndOutput();

    //�˳����̣߳�������Դ
	exitServer();

	return 0;
}
