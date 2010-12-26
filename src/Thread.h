// a thin wrapper around pthreads

#ifndef _THREAD_H_
#define _THREAD_H_

class Thread
{
public:
	Thread();
	~Thread();
	void Create(void* func(void*));
	void Cancel();
	void Lock();
	void Unlock();

	pthread_t handle;
	pthread_mutex_t mutex;
};

#endif
