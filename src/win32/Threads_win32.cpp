#include "ThreadGroup.h"
#include "Threads_win32.h"
#include <windows.h>

ThreadImpl *CreateThreadImpl()
{
	return new ThreadImpl_Win32;
}

void ThreadImpl_Win32::Start( THREAD_FUNC_ARG(func) )
{
	this->m_pThreadhandle = CreateThread( NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(func), NULL, 0, NULL );
}

void ThreadImpl_Win32::Pause()
{
	SuspendThread(this->m_pThreadhandle);
}

void ThreadImpl_Win32::Wait()
{
	WaitForSingleObject(this->m_pThreadhandle, INFINITE);
}