#ifndef _THREADS_WIN32_H
#define _THREADS_WIN32_H

#include "ThreadGroup.h"
#include <windows.h>

class ThreadImpl_Win32: public ThreadImpl
{
public:
	void Start( THREAD_FUNC_ARG(func) );
	void Pause();
	void Wait();
	
private:
	HANDLE m_pThreadhandle;
};

#endif /* _THREADS_WIN32_H */