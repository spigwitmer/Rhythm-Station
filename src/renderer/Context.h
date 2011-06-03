#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "utils/Singleton.h"

class Context : public Singleton<Context>
{
public:
	Context();
	virtual ~Context();

	void Init();

protected:
	int m_MaxAnisotropy,
		m_MaxAttributes,
		m_MaxUniforms;
};

#endif

