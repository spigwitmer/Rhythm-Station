#include "Texture.h"
#include "ResourceManager.h"
#include "PNGFile.h"

Texture::Texture() :
	ptr(0), path(), width(0),
	height(0), refcount(1)
{
	// init only
}

void Texture::Load(std::string path)
{
	if (!ResourceManager::GetResource(path,*this))
	{
		PNGFile png;
		*this = png.Load(path);

		// register this so we don't load it again.
		ResourceManager::Add(*this);
	}
}

void Texture::Delete()
{
	glDeleteTextures(1, &ptr);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, ptr);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * Created on 3/15/11
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
