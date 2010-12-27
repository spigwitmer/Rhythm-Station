#include <pthread.h>
#include "Thread.h"

Thread::Thread()
{
	pthread_mutex_init(&mutex,NULL);
}

Thread::~Thread()
{
	pthread_mutex_destroy(&mutex);
}

void Thread::Create(void* func(void*))
{
	pthread_create(&handle, NULL, func, (void*)this);
}

void Thread::Cancel()
{
	pthread_cancel(handle);
}

void Thread::Lock()
{
	pthread_mutex_lock(&mutex);
}

void Thread::Unlock()
{
	pthread_mutex_unlock(&mutex);
}
