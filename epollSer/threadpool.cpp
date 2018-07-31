/*
 * threadpool.cpp
 *
 *  Created on: 2018年7月29日
 *      Author: garvin
 */

#include"threadpool.h"

queue<task*> threadpool::m_taskqueue;

bool threadpool::shutdown = false;

pthread_mutex_t threadpool::m_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t threadpool::m_cond = PTHREAD_COND_INITIALIZER;

threadpool::threadpool(int threadnum){
	this->m_threadnum = threadnum;
	create();
}

void* threadpool::threadfunc(void* threaddata){
	pthread_t self_id = pthread_self();
	while(true){
		pthread_mutex_lock(&m_mutex);
		while(m_taskqueue.size()==0 && !shutdown)
			pthread_cond_wait(&m_cond, &m_mutex);
		if(shutdown){
			pthread_mutex_unlock(&m_mutex);
			pthread_exit(NULL);
		}
		task* tsk = m_taskqueue.front();
		m_taskqueue.pop();
		pthread_mutex_unlock(&m_mutex);
		tsk->run();
	}
	return (void*)0;
}

int threadpool::addtask(task* tsk){
	this->m_taskqueue.push(tsk);
	pthread_cond_signal(&m_cond);
	return 0;
}

int threadpool::create(){
	m_pthread_id = new pthread_t[m_threadnum];
	for(int i=0; i<m_threadnum; i++){
		pthread_create(m_pthread_id+i, NULL, threadfunc, NULL);
	}
	cout<<"线程池创建完成"<<endl;
	return 0;
}

int threadpool::gettasknum(){
	return this->m_taskqueue.size();
}

int threadpool::stopall(){
	if(shutdown)
		return -1;
	shutdown = true;
	pthread_cond_broadcast(&m_cond);
	for(int i=0; i<m_threadnum; i++){
		pthread_join(m_pthread_id[i], NULL);
	}
	delete[] m_pthread_id;
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
	return 0;
}
