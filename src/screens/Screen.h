#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <string>
#include <glm/glm.hpp>
#include "managers/ScreenManager.h"

class Screen
{
public:
	Screen(std::string sName);
	virtual ~Screen() {}

	void Update(double delta);
	void Draw();

	std::string GetName() const { return m_name; }
	glm::mat4 *GetProjection() { return &m_projection; }

	// For derived classes only.
	virtual void Init() = 0;
	virtual void UpdateInternal(double delta) = 0;

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
