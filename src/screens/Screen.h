#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <string>
#include "managers/ScreenManager.h"
#include <glm/glm.hpp>

class Screen
{
public:
	Screen(std::string sName) { }
	void Update(double delta);
	void Draw();

	glm::mat4 *GetProjection() { return &m_projection; }

protected:
	std::string m_name;
	glm::mat4 m_projection;
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
