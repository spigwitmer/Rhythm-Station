#include "RStation.h"
#include "managers/InputManager.h"
#include "managers/ScreenManager.h"
#include "utils/Logger.h"

using namespace std;

RStation::RStation() : m_status(RS_SUCCESS)
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
	SAFE_DELETE( LOG );
}

int RStation::Start(vector<string> vArgs)
{
	m_vArgs = vArgs;

	m_window = glfwOpenWindow(854, 480, GLFW_WINDOWED, "Rhythm-Station", 0);
	if (!glfwIsWindow(m_window))
	{
		LOG->Warn("Unable to create an OpenGL window. Check your drivers.");
		return RS_NO_WINDOW;
	}

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
		if (!glfwIsWindow(m_window))
			break;

		if (input->GetButton(RS_KEY_ESC)->IsDown())
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
