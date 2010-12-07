#include <GL/glew.h>
#include "RenderManager.h"
#include "Logger.h"
#include <sstream>

RenderManager* Renderer = NULL;

RenderManager::RenderManager() {
	glewInit();

	/*
	 * Don't pass in GetExtensions as a format arg - it could potentially be very,
	 * very long and use up the buffer.
	 */
	Log->Print("Available OpenGL extensions: \n" + GetExtensions());

	if (GLEW_NV_framebuffer_multisample_coverage) {
		Log->Print("CSAA Supported.");
		IsExtSupported["CSAA"] = true;
	}
	else
		Log->Print("CSAA Not Supported.");

	// Make transparency work!
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

std::string RenderManager::GetExtensions() {
	std::ostringstream extensions;
	extensions << glGetString(GL_EXTENSIONS);
	return extensions.str();
}

void RenderManager::BindBuffers(GLuint* buffers, Shader* shader) {
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

	shader->Bind();
	shader->SetUniforms();
}

/*******************************************************************
 * A FOREST OF TODO
 *******************************************************************/

//GLuint framebuffer, depth_tex;
//int shadowMapWidth = 1024, shadowMapHeight = 1024;

GLuint* RenderManager::CreateFramebuffer(vec2 size, int samples) {
	// TODO
	if (IsExtSupported["CSAA"]) {
		// 8x = 8, 4; 8xQ = 8, 8; 16x = 16, 4; 16xQ = 16, 8
//		glRenderbufferStorageMultisampleCoverageNV(buffer, 8, 4, GL_RGB, 0, 0);
	}
	return 0;
/*
	// needs cleanup! + color buffer and whatnot.
	glGenTextures(1, &depth_tex);
	glBindTexture(GL_TEXTURE_2D, depth_tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffersEXT(1, &framebuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);

	// we're not using a color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, depth_tex, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
		Log::Print("Can't create FBO.");
*/
}

void RenderManager::BindFramebuffer(GLuint* buffers) {
	// TODO
	/*
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glViewport(0, 0, shadowMapWidth, shadowMapHeight);
	glCullFace(GL_FRONT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	*/
}

// switch to the back buffer
void RenderManager::UnbindFramebuffer() {
/*
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);
	glCullFace(GL_BACK);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, g_width, g_height);
//	glLoadIdentity();
*/
}

void RenderManager::DeleteFramebuffer(GLuint* buffers) {
	// TODO
/*
	glDeleteTextures(1, &depth_tex);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDeleteFramebuffersEXT(1, &framebuffer);
*/
}
/*
void Framebuffer::BindTexture() {
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, depth_tex);
	glActiveTexture(GL_TEXTURE0);
}
*/
