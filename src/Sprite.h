#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "Shader.h"
#include "Message.h"
#include "Texture.h"
#include "Type.h"
#include "Mesh.h"
#include "Object.h"
#include <vector>
#include <map>

class Sprite : public Object
{
public:
	Sprite();
	virtual ~Sprite();
	
	void Load(std::string path);
	
	void Draw();
	void DrawPrimitives();
	
protected:	
	Mesh mesh;
	GLuint m_color_uniform;
		
	bool m_bDepthClear;
	
	rgba	m_color;
	Shader	m_shader;
	Texture	m_texture;
};

void Sprite_Binding();

#endif
