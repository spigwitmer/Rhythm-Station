#include "RStation.h"
#include "managers/ScreenManager.h"
#include "utils/Logger.h"
#include <GL/glfw3.h>

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

	return Loop();
}

int RStation::Loop()
{
	ScreenManager *screen = ScreenManager::GetSingleton();

	while (true)
	{
		// ScreenManager automatically calculates delta.
		screen->Update(glfwGetTime());
		screen->Draw();
		
		break;
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
