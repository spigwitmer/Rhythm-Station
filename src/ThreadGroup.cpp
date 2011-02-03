#include <vector>
#include "ThreadGroup.h"

RSThread::RSThread( THREAD_FUNC_ARG(func) )
{
	m_impl(func);
}
RSThread::~RSThread( void *func() )
{
	m_impl.Wait();
	//m_impl.Destroy();
}

void RSThread::Start() { m_impl.Start(); }
void RSThread::Pause() { m_impl.Pause(); }
void RSThread::Wait() { m_impl.Wait(); }

RSMutex::RSMutex()
{
	m_impl.Init();
}
RSMutex::~RSMutex()
{
	m_impl.Destroy();
}
