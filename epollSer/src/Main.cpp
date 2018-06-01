/*
 * Main.cpp
 *
 *  Created on: 2018年6月1日
 *      Author: garvin
 */
#include"global.h"

int main()
{
	InitGlobal();

    if(!InitSocket())
    {
    	EXIT();
    }

    if(!StartEpoll())
    {
    	EXIT();
    }

    Epoll();

    return 0;
}


