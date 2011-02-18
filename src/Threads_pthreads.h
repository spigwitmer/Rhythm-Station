#ifndef _THREADS_PTHREADS_H
#define _THREADS_PTHREADS_H

#include "ThreadGroup.h"
#include <pthread.h>

class ThreadImpl_pthreads: public ThreadImpl
{
public:
	//ThreadImpl_pthreads();
	//~ThreadImpl_pthreads();
	void Start( THREAD_FUNC_ARG(func) );
	void Pause();
	void Wait();

private:
	//pthread_attr_t m_attr;
	pthread_t m_pThreadhandle;
};

#endif /* _THREADS_PTHREADS_H */
