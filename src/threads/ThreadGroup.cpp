#include <vector>
#include "ThreadGroup.h"

RSThread::RSThread()
{
}
RSThread::~RSThread()
{
}

void RSThread::start(THREAD_FUNC_ARG(func))
{
	m_impl = CreateThreadImpl();
	m_impl->start(func);
}

void RSThread::join() { m_impl->join(); }
