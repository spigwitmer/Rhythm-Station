#ifndef _SCREEN_MANAGER_H_
#define _SCREEN_MANAGER_H_

#include <vector>
#include <string>
#include <map>

#include "utils/Singleton.h"

class Screen
{
public:
	void Update() { }
};

class ScreenManager : public Singleton<ScreenManager>
{
public:
	void Update();
	void Draw();
	
	void PushScreen();
	void PopScreen();
	
	Screen* GetTopScreen();

private:
	std::vector<Screen*> m_vScreenStack;
	std::map<std::string, Screen*> m_vScreenTypes;
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
