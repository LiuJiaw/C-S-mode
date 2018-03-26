#include "TcpSer.h"


int main()
{
    //初始化全局变量
    InitGlobal();
	//初始化套接字
	if (!initSocket())
	{
		exitServer();
    	return -1;
	}

	if(createCleanAndAcceptThread())	//接受客户端请求的线程
    {
        showServerStartMsg(true);		//创建线程成功信息
    }
    else
    {
        showServerStartMsg(false);
		exitServer();
    	return -1;
    }

	 //处理数据
    inputAndOutput();

    //退出主线程，清理资源
	exitServer();

	return 0;
}
