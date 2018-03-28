#include "TcpSer.h"


int main()
{
    //初始化全局变量
    InitGlobal();
    //初始化套接字
    if (!initSocket())
    {
	Exit();
    	return -1;
    }
    //接受客户端请求的线程
    if(createThread())	
    {
        showServerStartMsg(true);
    }
    else
    {
        showServerStartMsg(false);
		Exit();
    	return -1;
    }
    //处理数据
    SendBuffer();
    //退出主线程，清理资源
    Exit();

    return 0;
}
