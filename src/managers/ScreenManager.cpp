#include "ScreenManager.h"

using namespace std;

void ScreenManager::Update()
{
	vector<Screen*>::iterator it;
	for (it = m_vScreenStack.begin(); it != m_vScreenStack.end(); it++)
	{
		(*it)->Update();
	}
}

void ScreenManager::Draw()
{

}

void ScreenManager::PushScreen()
{
	m_vScreenStack.push_back(new Screen());
}

void ScreenManager::PopScreen()
{
	delete m_vScreenStack.back();
	m_vScreenStack.pop_back();
}

Screen* ScreenManager::GetTopScreen()
{
	return m_vScreenStack.back();
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
