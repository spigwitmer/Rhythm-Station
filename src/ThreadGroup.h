#include <pthread.h>
#include <vector>

#ifndef _THREADGROUP_H_
#define _THREADGROUP_H_

class ThreadGroup;

typedef struct _ThreadParameters
{
	ThreadGroup	*group;
	void		*argument;
} ThreadParameters;

typedef struct _Thread
{
	pthread_t			handle;
	ThreadParameters	parameters;
} Thread;

class ThreadGroup
{
private:
	pthread_mutexattr_t attr;
	pthread_mutex_t lock;
	std::vector<Thread *> threads;
	
public:
	ThreadGroup();
	~ThreadGroup();
	
	int CreateThread(void *func(void *),void *arg);
	int JoinAll();
	int Lock();
	int Unlock();
};

#endif
