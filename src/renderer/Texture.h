#ifndef _TEXTURE_H_
#define _TEXTURE_H_

class Texture
{
public:
	Texture();
	virtual ~Texture();
};

// For Render-to-Texture
class Framebuffer
{
public:
	Framebuffer();
	virtual ~Framebuffer();

	Texture *GetTexture() { return &m_Texture; }

private:
	Texture m_Texture;
};

#endif

