#ifndef _THREADS_PTHREADS_H
#define _THREADS_PTHREADS_H

#include "ThreadGroup.h"
#include <pthread.h>

class ThreadImpl_pthreads: public ThreadImpl
{
public:
	virtual ~ThreadImpl_pthreads();
	void start(THREAD_FUNC_ARG(func));
	void join();

private:
	pthread_t m_pThreadhandle;
};

#endif /* _THREADS_PTHREADS_H */
