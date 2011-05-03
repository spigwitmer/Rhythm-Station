#ifndef _FONT_H_
#define _FONT_H_

#include "Object.h"
#include "../Mesh.h"
#include <string>

class Font : public Object
{
public:
	void Load(std::string, int size);
	void Set(std::string);
	void DrawPrimitives();
	
private:
	std::string m_text;
	Mesh mesh;
};

#endif
