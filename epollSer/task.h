/*
 * task.h
 *
 *  Created on: 2018年7月29日
 *      Author: garvin
 */

#ifndef TASK_H_
#define TASK_H_

#include<string.h>
#include<unistd.h>
#include<iostream>
#include<netdb.h>
#include<sstream>

using namespace std;

class task{
public:
	task(){}
	virtual void run() = 0;
	virtual ~task(){};
};

class getinfotask:public task{
private:
	char recvstr[50];
	int len;
	int clisockfd;
public:
	getinfotask(){}
	void setrecvstr(char* recv_str, int length){
		memcpy(recvstr, recv_str, strlen(recv_str));
		this->len = length;
		*(recvstr+len-1) = '\0';
	}
	void setsockfd(int sock_fd){
		this->clisockfd = sock_fd;
	}
	void run(){
	    
	}
	~getinfotask(){}
};



#endif /* TASK_H_ */
