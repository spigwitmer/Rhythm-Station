#include <GL/glew.h>
#include "RenderManager.h"
#include <sstream>

RenderManager* Renderer = NULL;

std::string RenderManager::GetExtensions() {
	std::ostringstream extensions;
	extensions << glGetString(GL_EXTENSIONS);
	return extensions.str();
}

// 8x = 8, 4; 8xQ = 8, 8; 16x = 16, 4; 16xQ = 16, 8 (this will be elsewhere)
// glRenderbufferStorageMultisampleCoverageNV(buffer, 8, 4, GL_RGB, 0, 0);
