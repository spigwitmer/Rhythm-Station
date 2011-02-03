#ifndef _THREADGROUP_H_
#define _THREADGROUP_H_

#include <vector>
#define THREAD_FUNC_ARG(x) void(##x*)() // this might change

class ThreadImpl
{
public:
	virtual ThreadImpl( THREAD_FUNC_ARG(func) ) = 0;
	virtual ~ThreadImpl() = 0;
	virtual void Start() = 0;
	virtual void Pause() = 0;
	virtual void Wait() = 0;
	//virtual void Destroy() = 0; // ..IT ALL MIGHT CHANGE
};

class MutexImpl
{
public:
	virtual MutexImpl() = 0;
	//virtual ~MutexImpl() = 0;
	virtual void Lock() = 0;
	virtual void Unlock() = 0;
};

class RSThread
{
public:
	RSThread( THREAD_FUNC_ARG(func) );
	~RSThread();

	void Start();
	void Pause();
	void Wait();
private:
	ThreadImpl m_impl;
};

class RSMutex
{
public:
	RSMutex();
	~RSMutex();

	void Lock();
	void Unlock();
private:
	MutexImpl m_impl;
};

class RSThreadGroup
{
private:
	RSMutex m_lock;
	std::vector<RSThread *> m_threads;
	
public:
	RSThreadGroup();
	~RSThreadGroup();
	
	int CreateThread(void *func(void *),void *arg);
	int PauseAll();
	int UnpauseAll();
};

#endif /* _THREADGROUP_H_ */
