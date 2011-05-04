#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "renderer/Shader.h"
#include "Message.h"
#include "renderer/Texture.h"
#include "Type.h"
#include "renderer/Mesh.h"
#include "Object.h"
#include <vector>
#include <map>

class Sprite : public Object
{
public:
	Sprite();
	virtual ~Sprite();
	
	void Load(std::string path);
	void DrawPrimitives();
	
protected:	
	Mesh mesh;		
	bool m_bDepthClear;
};

void Sprite_Binding();

#endif
