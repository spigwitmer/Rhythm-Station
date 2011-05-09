#include "RStation.h"

using namespace std;

RStation::RStation()
{

}

RStation::~RStation()
{

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
		screen->Update();
		screen->Draw();
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
