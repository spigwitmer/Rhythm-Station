#ifndef _THREADGROUP_H_
#define _THREADGROUP_H_

#include <vector>
#define THREAD_FUNC_ARG(xarg) void* (*xarg)(void *) // REMOVE THIS ALTOGETHER

class ThreadImpl
{
public:
	virtual void Start( THREAD_FUNC_ARG(func) ) = 0;
	virtual void Pause() = 0;
	virtual void Wait() = 0;
	//virtual void Destroy() = 0; // ..IT ALL MIGHT CHANGE

protected:
	void *m_pData;
};

ThreadImpl *CreateThreadImpl();

/*
class MutexImpl
{
public:
	virtual MutexImpl();
	//virtual ~MutexImpl() = 0;
	virtual void Lock() = 0;
	virtual void Unlock() = 0;
};
*/

class RSThread
{
public:
	RSThread();
	~RSThread();

	void Start( THREAD_FUNC_ARG(func) );
	void Pause();
	void Wait();
private:
	ThreadImpl *m_impl;
};

/*
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
*/
/*
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
};*/

#endif /* _THREADGROUP_H_ */
