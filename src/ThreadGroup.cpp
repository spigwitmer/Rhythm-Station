#include <pthread.h>
#include <vector>
#include "ThreadGroup.h"


ThreadGroup::ThreadGroup()
{
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
//	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_ERRORCHECK);
	pthread_mutex_init(&lock,&attr);
}

ThreadGroup::~ThreadGroup()
{
	if (!threads.empty())
		JoinAll();
	pthread_mutex_destroy(&lock);
	pthread_mutexattr_destroy(&attr);
}

int ThreadGroup::CreateThread(void *func(void*),void *arg)
{
	Thread* thread = new Thread;
	threads.push_back(thread);
	thread->parameters.group = this;
	thread->parameters.argument = arg;
	int rc = pthread_create(&(thread->handle),NULL,func,&(thread->parameters));
	return rc;
}

int ThreadGroup::JoinAll()
{
	while (!threads.empty())
	{
		int rc = pthread_join(threads.back()->handle,NULL);
		if (rc == 0)
		{
			delete threads.back();
			threads.pop_back();
		}
		else
		{
			return rc;
		}
	}
	return 0;
}

int ThreadGroup::Lock()
{
	return pthread_mutex_lock(&lock);
}

int ThreadGroup::Unlock()
{
	return pthread_mutex_unlock(&lock);
}
