#include <GL/glew.h>
#include "RTT.h"
#include "RStation.h"
#include "ShaderLoader.h"
#include "Primitives.h"

GLuint color_tex, framebuffer, depth_rb;

ShaderLoader *post;

void create_fbo()
{
	post = new ShaderLoader();
	post->Load("radial_blur.vert","radial_blur.frag");
	glGenTextures(1, &color_tex);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 854, 480, 0, GL_BGRA, GL_FLOAT, NULL);

	glGenFramebuffersEXT(1, &framebuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);

	glFramebufferTexture2DEXT(
		GL_FRAMEBUFFER_EXT,
		GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D, color_tex, 0
	);

	glGenRenderbuffersEXT(1, &depth_rb);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 854, 480);

	glFramebufferRenderbufferEXT(
		GL_FRAMEBUFFER_EXT,
		GL_DEPTH_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT, depth_rb
	);

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		Log::Print("FBO error");
		return;
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
}

void draw_fbo()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // return to back buffer
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, get_framebuffer_tex());
	post->Bind();
	// XXX
	glDisable(GL_CULL_FACE);
	Primitive::Quad(vec2(854,480));
	glEnable(GL_CULL_FACE);
	post->Unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void delete_fbo()
{
	glDeleteTextures(1, &color_tex);
	glDeleteRenderbuffersEXT(1, &depth_rb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDeleteFramebuffersEXT(1, &framebuffer);
}

GLuint get_framebuffer() { return framebuffer; }
GLuint get_framebuffer_tex() { return color_tex; }

void check_gl_errors()
{
	GLenum err = glGetError();
	switch (err) {
		case GL_OUT_OF_MEMORY:
			Log::Print("GL_OUT_OF_MEMORY");
			break;
		default:
			if (err)
				printf("GL ERROR: %i\n",err);
			break;
	}
}