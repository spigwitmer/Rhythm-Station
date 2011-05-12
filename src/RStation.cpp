#include <GL/glew.h>
#include "RStation.h"
#include "managers/InputManager.h"
#include "managers/ScreenManager.h"
#include "utils/Logger.h"

using namespace std;

RStation::RStation() :
	m_status(RS_SUCCESS),
	m_window(0)
{
	LOG = new Logger();
	int err = glfwInit();
	if (!err)
	{
		LOG->Warn(glfwErrorString(err));
		m_status = RS_INIT_FAILURE;
	}
}

RStation::~RStation()
{
	SAFE_DELETE(LOG);
}

int RStation::Start(vector<string> vArgs)
{
	m_vArgs = vArgs;

	glfwOpenWindowHint(GLFW_DEPTH_BITS, 24);
	glfwOpenWindowHint(GLFW_STENCIL_BITS, 8);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 0);
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, 1);
	m_window = glfwOpenWindow(960, 540, GLFW_WINDOWED, "Rhythm-Station", 0);

	// Make sure we were able to create a rendering context.
	if (!glfwIsWindow(m_window))
	{
		LOG->Warn("Unable to create an OpenGL window. Check your drivers.");
		return RS_NO_WINDOW;
	}

	glewInit();

	// Enable 8 attributes for all our vertex data.
	for (int i = 0; i<8; i++)
	{
		glEnableVertexAttribArray(i);
	}
	
	int attrs = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attrs);
	LOG->Info("Hardware supports %d vertex attributes.", attrs);

	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &attrs);
	LOG->Info("Hardware supports %d uniform components.", attrs);

	return Loop();
}

int RStation::Loop()
{
	ScreenManager *screen = ScreenManager::GetSingleton();
	InputManager *input = InputManager::GetSingleton();

	while (true)
	{
		// Break if user closed the window
		input->Update();
		if (!glfwIsWindow(m_window) || input->GetButton(RS_KEY_ESC)->IsDown())
			break;

		// ScreenManager automatically calculates delta.
		screen->Update(glfwGetTime());
		screen->Draw();

		glfwSwapBuffers();
	}

	return m_status;
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
