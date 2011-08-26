#ifndef _RSTATION_H_
#define _RSTATION_H_

#include <vector>
#include <string>
#include <GL/glfw3.h>

// Helps prevent heap corruption.
#define SAFE_DELETE(p) if( (p) != NULL ) delete (p); (p) = NULL;

class RStation
{
public:
	RStation(std::vector<std::string> &vArguments);
	virtual ~RStation();

	int Run();

private:
	std::vector<std::string> m_vArgs;

	GLFWwindow m_window;
};

#endif

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
