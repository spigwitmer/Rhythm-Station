#ifndef _TEXTURE_H_
#define _TEXTURE_H_

class Texture
{
public:
	Texture();

	unsigned int width, height, id, refcount;
};

// For Render-to-Texture
class Framebuffer : public Texture
{
public:
	Framebuffer();

	Texture *GetTexture() { return &m_Texture; }
	
private:
	Texture m_Texture;
};

#endif
