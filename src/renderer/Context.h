#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "utils/Singleton.h"

class Context : public Singleton<Context>
{
public:
	Context();
	virtual ~Context();

	void Init(bool using_gl3);

protected:
	int m_MaxAnisotropy,
		m_MaxAttributes,
		m_MaxUniforms,
		m_MaxTextureSize;
};

#endif

