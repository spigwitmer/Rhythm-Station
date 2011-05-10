#include "ScreenManager.h"
#include "screens/Screen.h"

using namespace std;

void ScreenManager::Update(double time)
{
	double delta = time - m_LastUpdate;
	m_LastUpdate = time;
	
	vector<Screen*>::iterator it;
	for (it = m_vScreenStack.begin(); it != m_vScreenStack.end(); it++)
	{
		(*it)->Update(delta);
	}
}

void ScreenManager::Draw()
{

}

void ScreenManager::PushScreen(string sName)
{
	m_vScreenStack.push_back(new Screen(sName));
}

void ScreenManager::PopScreen()
{
	delete m_vScreenStack.back();
	m_vScreenStack.pop_back();
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
