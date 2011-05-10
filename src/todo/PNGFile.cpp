#include <GL/glfw3.h>
#include <png.h>
#include "managers/FileManager.h"
#include "PNGFile.h"
#include "utils/Logger.h"

Texture PNGFile::Load(std::string _path)
{
	Texture tex;
	
	// png specifics
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL, end_info = NULL;
	png_bytep *row_pointers = NULL;
	png_byte sig[8];
	
	// the rest
	FILE *pngFile = NULL;
	GLubyte *pixels = NULL;
	GLuint glformat = 0;
	int components, rowsize;
	tex.path = _path;
	pngFile = fopen(_path.c_str(), "rb");
	
	if (!pngFile)
	{
		Log->Print("File \"%s\" not found.", tex.path.c_str());
		return Texture();
	}
	
	if (fread(&sig, sizeof(png_byte), 8, pngFile) < 8)
	{
		Log->Print("Header of the file \"%s\" couldn't be fully read!", tex.path.c_str());
		fclose(pngFile);
		return Texture();
	}
	
	if (png_sig_cmp(sig, 0, 8))
	{
		Log->Print("File \"%s\" is not a valid png file!", tex.path.c_str());
		fclose(pngFile);
		return Texture();
	}
	
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,NULL,NULL);
	
	if (!png_ptr)
	{
		fclose(pngFile);
		return Texture();
	}
	
	info_ptr = png_create_info_struct(png_ptr);
	
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(pngFile);
		return Texture();
	}
	
	end_info = png_create_info_struct(png_ptr);
	
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(pngFile);
		return Texture();
	}
	
	// safety first. :)
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(pngFile);
		return Texture();
	}
	
	// init io and tell libpng we've already read the sig.
	png_init_io(png_ptr, pngFile);
	png_set_sig_bytes(png_ptr, 8);
	
	// If we've got tRNS on an RGB image, it's a color key. Make this RGBA.
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);
		
	// filler for keys
	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
	
	// read the file
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
	tex.width = png_get_image_width(png_ptr,info_ptr);
	tex.height = png_get_image_height(png_ptr,info_ptr);
	rowsize = png_get_rowbytes(png_ptr,info_ptr);
	components = png_get_channels(png_ptr,info_ptr);
	row_pointers = png_get_rows(png_ptr, info_ptr);
	pixels = new GLubyte[rowsize * tex.height];
	
	for (unsigned int i = 0; i < tex.height; i++)
		memcpy(&pixels[i*rowsize], row_pointers[i], rowsize);
		
	// prepare texture for uploading to GPU
	glGenTextures(1, &tex.ptr); // make it
	glBindTexture(GL_TEXTURE_2D, tex.ptr); // bind it
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	switch(components)
	{
	case 1:
		glformat = GL_LUMINANCE;
		break;
	case 2:
		glformat = GL_LUMINANCE_ALPHA;
		break;
		// case 3 would be converted to 4, above.
	case 4:
		glformat = GL_RGBA;
		break;
	default:
		glformat = 0; // this shouldn't happen.
		break;
	}
	
	// log some stats
	Log->Print("Uploading texture (res = %dx%d, channels = %d)", tex.width, tex.height, components);
	
	// finally upload the texture
	glTexImage2D(GL_TEXTURE_2D, 0, components, tex.width, tex.height, 0, glformat, GL_UNSIGNED_BYTE, pixels);

	// cleanup
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] pixels;
	fclose(pngFile);
	return tex;
}


/**
 * @file
 * @author Colby Klein, Jarno Purontakanen (c) 2011
 * @section LICENSE
 * 
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
