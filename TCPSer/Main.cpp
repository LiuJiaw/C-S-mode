#include "TcpSer.h"


int main()
{
    //��ʼ��ȫ�ֱ���
    InitGlobal();
	//��ʼ���׽���
	if (!initSocket())
	{
		Exit();
    	return -1;
	}

	if(createThread())	//���ܿͻ���������߳�
    {
        showServerStartMsg(true);		//�����̳߳ɹ���Ϣ
    }
    else
    {
        showServerStartMsg(false);
		Exit();
    	return -1;
    }

	 //��������
    SendBuffer();

    //�˳����̣߳�������Դ
	Exit();

	return 0;
}
