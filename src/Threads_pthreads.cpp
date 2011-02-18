#include "Threads_pthreads.h"
#include "ThreadGroup.h"
#include <pthread.h>

ThreadImpl *CreateThreadImpl()
{
	return new ThreadImpl_pthreads; // TODO: memleak much?
}

/*
ThreadImpl_pthreads::ThreadImpl_pthreads()
{
	//pthread_attr_init(&m_attr);
}

ThreadImpl_pthreads::~ThreadImpl_pthreads()
{
}
*/

void ThreadImpl_pthreads::Start( THREAD_FUNC_ARG(func) )
{
	pthread_create(&m_pThreadhandle, NULL, func, NULL);
}

void ThreadImpl_pthreads::Pause()
{
/* Lots of ptrace stuff here */
}

void ThreadImpl_pthreads::Wait()
{
	pthread_join(m_pThreadhandle, NULL);
}
