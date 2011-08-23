#include "Threads_pthreads.h"
#include "ThreadGroup.h"
#include <pthread.h>

ThreadImpl *CreateThreadImpl()
{
	return new ThreadImpl_pthreads; // TODO: memleak much?
}

ThreadImpl_pthreads::~ThreadImpl_pthreads()
{
}

void ThreadImpl_pthreads::start(THREAD_FUNC_ARG(func))
{
	pthread_create(&m_pThreadhandle, NULL, func, NULL);
}

void ThreadImpl_pthreads::join()
{
	pthread_join(m_pThreadhandle, NULL);
}
