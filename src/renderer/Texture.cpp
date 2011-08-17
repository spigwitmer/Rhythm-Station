#include "Texture.h"
#include <OpenGL/gl3.h>

Texture::Texture() : width(0), height(0), id(0), refcount(1)
{
}

Framebuffer::Framebuffer() : m_Texture()
{
}
