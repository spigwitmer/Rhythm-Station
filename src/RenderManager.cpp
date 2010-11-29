#include <GL/glew.h>
#include "RenderManager.h"
#include "Logger.h"
#include <sstream>

RenderManager* Renderer = NULL;

RenderManager::RenderManager() {
	glewInit();

	Log->Print("Available OpenGL extensions: \n" + GetExtensions());

	if (GLEW_NV_framebuffer_multisample_coverage) {
		Log->Print("CSAA Supported.");
		IsExtSupported["CSAA"] = true;

		// 8x = 8, 4; 8xQ = 8, 8; 16x = 16, 4; 16xQ = 16, 8 (this will be elsewhere)
		// glRenderbufferStorageMultisampleCoverageNV(buffer, 8, 4, GL_RGB, 0, 0);
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

GLuint* RenderManager::CreateFramebuffer(int samples) {
	// TODO
	return 0;
}

void RenderManager::BindFramebuffer(GLuint* buffers) {
	// TODO
}

void RenderManager::DeleteFramebuffer(GLuint* buffers) {
	// TODO
}
