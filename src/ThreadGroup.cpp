#include <vector>
#include "ThreadGroup.h"

RSThread::RSThread()
{
}
RSThread::~RSThread()
{
	//m_impl.Wait();
	//m_impl.Destroy();
}

void RSThread::Start( THREAD_FUNC_ARG(func) )
{
	m_impl = CreateThreadImpl();
	m_impl->Start(func);
}
void RSThread::Pause() { m_impl->Pause(); }
void RSThread::Wait() { m_impl->Wait(); }

/*
RSMutex::RSMutex()
{
	m_impl->Init();
}
RSMutex::~RSMutex()
{
	m_impl->Destroy();
}*/


