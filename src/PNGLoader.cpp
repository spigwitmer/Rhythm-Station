#include <GL/glfw3.h>
#include <png.h>
#include "FileManager.h"
#include "PNGLoader.h"
#include "Logger.h"

Texture PNGLoader::Load(std::string _path) {
	Texture tex;

	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_infop end_info = NULL;
	png_bytep *row_pointers = NULL;
	int components, rowsize;

	tex.path = _path;

	FILE *pngFile = fopen(_path.c_str(), "rb");

	if (!pngFile) {
		Log->Print("[PNGLoader::Load] File \"" + tex.path + "\" not found.");
		return Texture();
	}

	png_byte sig[8];

	fread(&sig, sizeof(png_byte), 8, pngFile);
	if (png_sig_cmp(sig, 0, 8)) {
		Log->Print("[PNGLoader::Load] File \"" + tex.path + "\" is not a valid png file!");
		fclose(pngFile);
		return Texture();
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,NULL,NULL);

	if (!png_ptr) {
		fclose(pngFile);
		return Texture();
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(pngFile);
		return Texture();
	}

	end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(pngFile);
		return Texture();
	}

	// I don't know if we need this but safety first. :)
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(pngFile);
		return Texture();
	}

	png_init_io(png_ptr, pngFile);

	// Tell libpng how much we did read already
	png_set_sig_bytes(png_ptr, 8);


	// The fast way
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	tex.width = png_get_image_width(png_ptr,info_ptr);
	tex.height = png_get_image_height(png_ptr,info_ptr);
	rowsize = png_get_rowbytes(png_ptr,info_ptr);
	components = png_get_channels(png_ptr,info_ptr);
	printf("res: %dx%d, channels: %d\n", tex.width, tex.height, components);

	row_pointers = png_get_rows(png_ptr, info_ptr);

	GLubyte *pixels = new GLubyte[rowsize * tex.height];
	for (unsigned i = 0; i < tex.height; i++)
		memcpy(&pixels[i*rowsize] +i , row_pointers[i], rowsize);
		// adding i after pixels fixes terminal.png, i*2 fixes test.png. I've got no idea why.
//		row_pointers[i] = (png_bytep)(pixels + (i * components * tex.width));


	// upload texture to GPU
	glGenTextures(1, &tex.ptr); // make it
	glBindTexture(GL_TEXTURE_2D, tex.ptr); // bind it
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	GLuint glformat;
	switch(components) {
		case 1:
			glformat = GL_LUMINANCE;
			break;
		case 2:
			glformat = GL_LUMINANCE_ALPHA;
			break;
		case 3:
			glformat = GL_RGB;
			break;
		case 4:
			glformat = GL_RGBA;
			break;
		default:
			glformat = 0; // this shouldn't happen.
			break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, components, tex.width, tex.height, 0, glformat, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	// register this so we don't load it again.
	Resources->Add(&tex);

	// cleanup
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] pixels;
	fclose(pngFile);
	
	return tex;
}
