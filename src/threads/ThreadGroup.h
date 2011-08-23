#ifndef _THREADGROUP_H_
#define _THREADGROUP_H_

#include <vector>
#define THREAD_FUNC_ARG(xarg) void* (*xarg)(void *) // REMOVE THIS ALTOGETHER

class ThreadImpl
{
public:
    // exists to shut up a compiler warning.
    virtual ~ThreadImpl() { };
	virtual void start( THREAD_FUNC_ARG(func) ) = 0;
	virtual void join() = 0;

protected:
	void *m_pData;
};

ThreadImpl *CreateThreadImpl();

class RSThread
{
public:
	RSThread();
	~RSThread();

	void start(THREAD_FUNC_ARG(func));
	void join();
private:
	ThreadImpl *m_impl;
};

#endif /* _THREADGROUP_H_ */
