/*
 * Main.cpp
 *
 *  Created on: 2018年6月1日
 *      Author: garvin
 */
#include"global.h"

int main() {
	InitGlobal();
	//初始化套接字
	if (!InitSocket()) {
		Exit();
	}
	//建立epoll监听队列
	//并将服务器监听套接字加入
	if (!StartEpoll()) {
		Exit();
	}
	cout << "epoll监听已开启" << endl;
	//对已加入监听队列的套接字进行监听
	//如接收、转发消息，加入、清离监听队列
	Epoll();

	return 0;
}

