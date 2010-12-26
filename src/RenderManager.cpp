#include <GL/glew.h>
#include <GL/glfw3.h>
#include "GameManager.h"
#include "RenderManager.h"
#include "Logger.h"
#include <sstream>

RenderManager* Renderer = NULL;

RenderManager::RenderManager()
{
	glewInit();

	glfwGetWindowSize(Game->GetWindow(), &ScreenWidth, &ScreenHeight);

	if (GLEW_NV_framebuffer_multisample_coverage)
	{
		Log->Print("CSAA Supported.");
		Game->IsExtSupported["CSAA"] = true;
	}
	else
		Log->Print("CSAA Not Supported.");

	// Make transparency work!
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

std::string RenderManager::GetExtensions()
{
	std::ostringstream extensions;
	extensions << glGetString(GL_EXTENSIONS);
	return extensions.str();
}
