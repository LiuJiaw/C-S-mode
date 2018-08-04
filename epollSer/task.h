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
		stringstream stream;
	    stream << "GET " << "/openapi/api" << "?" << "key=cb41d21f0e0440d38b435bf16790bb43&info="<<recvstr;
	    stream << " HTTP/1.0\r\n";
	    stream << "Host: " << "www.tuling123.com" << "\r\n";
	    stream <<"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	    stream <<"Connection:close\r\n\r\n";
	    int sockfd;
	    struct sockaddr_in address;
	    struct hostent *server;
	    sockfd = socket(AF_INET,SOCK_STREAM,0);
	    address.sin_family = AF_INET;
	    address.sin_port = htons(80);
	    server = gethostbyname("www.tuling123.com");
	    memcpy((char *)&address.sin_addr.s_addr,(char*)server->h_addr, server->h_length);
	    if(-1 == connect(sockfd,(struct sockaddr *)&address,sizeof(address))){
	        cout <<"connection error!"<<endl;
	        return;
	    }

	    send(sockfd,stream.str().c_str(),stream.str().size(), 0);
	    string buf = "";
	    buf.resize(1024);
	    int offset = 0;
	    int rc;

	    while(rc = recv(sockfd, (char*)&buf[offset], 1024, 0))
	    {
	        offset += rc;
	    }

	    close(sockfd);

	    buf[offset] = '\0';
	    int pos1 = buf.find("\"text\"");
	    int pos2 = buf.find("\"}");
	    send(clisockfd, &buf[pos1+8], pos2-pos1-8, 0);
	}
	~getinfotask(){}
};



#endif /* TASK_H_ */
