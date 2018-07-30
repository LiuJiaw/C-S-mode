/*
 * threadpool.h
 *
 *  Created on: 2018年7月29日
 *      Author: garvin
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include<pthread.h>
#include<queue>
#include"task.h"

class threadpool{
private:
	static queue<task*> m_taskqueue;
	int m_threadnum;
	pthread_t *m_pthread_id;
	static bool shutdown;
	static pthread_mutex_t m_mutex;
	static pthread_cond_t m_cond;

	static void* threadfunc(void* threaddata);
	int create();
public:
	threadpool(int threadnum);
	int addtask(task* tsk);
	int stopall();
	int gettasknum();
};



#endif /* THREADPOOL_H_ */
