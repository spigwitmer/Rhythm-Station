#include <GL/glfw3.h>
#include <png.h>
#include "FileManager.h"
#include "PNGLoader.h"
#include "Logger.h"

Texture PNGLoader::Load(std::string _path) {
	Texture tex;
	tex.path = _path;

	int bitDepth, format;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytep *row_pointers = NULL;
	FILE *pngFile = NULL;

	// make sure the file exists!
	if (File->FileExists(_path)) {
		if(!(pngFile = fopen(_path.c_str(), "rb"))) {
			Log->Print("Error opening file \"" + _path + "\"");
			return Texture();
		}
	}
	else {
		Log->Print("File \"" + _path + "\" not found.");
		return Texture();
	}

	png_byte sig[8];
	fread(&sig, 8, sizeof(png_byte), pngFile);
	rewind(pngFile); // so when we init io it won't bitch

	if (!png_check_sig(sig, 8))
		return Texture();

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,NULL,NULL);

	if (!png_ptr || setjmp(png_jmpbuf(png_ptr)) ||
		!(info_ptr = png_create_info_struct(png_ptr)))
		return Texture();

	png_init_io(png_ptr, pngFile);
	png_read_info(png_ptr, info_ptr);

	// some of the stuff below here seems to not make any difference.
	bitDepth = png_get_bit_depth(png_ptr, info_ptr);
	format = png_get_color_type(png_ptr, info_ptr);
	if (format == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	if (format == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
		png_set_expand_gray_1_2_4_to_8(png_ptr);

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	if (bitDepth == 16)
		png_set_strip_16(png_ptr);
	else if (bitDepth < 8)
		png_set_packing(png_ptr);

	png_read_update_info(png_ptr, info_ptr);
	png_uint_32 width, height;
	png_get_IHDR(png_ptr, info_ptr, &width, &height,
			 &bitDepth, &format, NULL, NULL, NULL);
	tex.width = width;
	tex.height = height;

	int components;
	switch (format) {
		case PNG_COLOR_TYPE_GRAY:
			components = 1;
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			components = 2;
			break;
		case PNG_COLOR_TYPE_RGB:
			components = 3;
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			components = 4;
			break;
		default:
			// if we're here, something is wrong!
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			Log->Print("File \"" + _path + "\" is invalid. Is this really a PNG file?");
			return Texture();
	}

	GLubyte *pixels = new GLubyte[tex.width * tex.height * components];
	row_pointers = new png_bytep[tex.height];

	printf("components = %d, depth = %d, res = %dx%d\n", components, bitDepth, tex.width, tex.height);

	for (unsigned i = 0; i < tex.height; i++)
		// adding i after pixels fixes terminal.png, i*2 fixes test.png. I've got no idea why.
		row_pointers[i] = (png_bytep)(pixels + (i * components * tex.width));

	png_read_image(png_ptr, row_pointers);
	png_read_end(png_ptr, NULL);

	// generate pointer, bind, set params, and upload texture to the GPU.
	glGenTextures(1, &tex.ptr);
	glBindTexture(GL_TEXTURE_2D, tex.ptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// switches take up too much space. time for a little voodoo.
	GLuint glcolors;
	(components == 4) ? (glcolors = GL_RGBA) : 0;
	(components == 3) ? (glcolors = GL_RGB) : 0;
	(components == 2) ? (glcolors = GL_LUMINANCE_ALPHA) : 0;
	(components == 1) ? (glcolors = GL_LUMINANCE) : 0;
	glTexImage2D(GL_TEXTURE_2D, 0, components, tex.width, tex.height, 0, glcolors, GL_UNSIGNED_BYTE, pixels);

	// unbind so things are as we found them.
	glBindTexture(GL_TEXTURE_2D, 0);

	// register this so we don't load it again.
	Resources->Add(&tex);

	// cleanup
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);	
	fclose(pngFile);
	delete[] row_pointers;
	delete[] pixels;
	
	return tex;
}
