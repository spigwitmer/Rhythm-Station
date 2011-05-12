#include "screens/Screen.h"
#include "utils/Logger.h"

using namespace std;

Screen::Screen(string sName) :
	m_name(sName),
	m_projection(1.0)
{
	LOG->Info("Created screen \"%s\"", sName.c_str());
}

Screen::~Screen()
{
}

void Screen::Update(double delta)
{
	// (Update stuff)

	UpdateInternal(delta);
}

void Screen::Draw()
{
	// TODO
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
