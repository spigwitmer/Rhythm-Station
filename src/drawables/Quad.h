#ifndef _QUAD_H_
#define _QUAD_H_

#include "Drawable.h"

class Quad : public Drawable
{
public:
	Quad();
	virtual ~Quad();

	void DrawInternal();

protected:
	// vertices + indices
	unsigned m_VBO[2];
	
	// make sure we don't draw if init failed.
	bool m_init;
};

#endif
