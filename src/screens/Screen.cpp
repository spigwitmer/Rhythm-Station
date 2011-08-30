#include "screens/Screen.h"
#include "utils/Logger.h"

using namespace std;

REGISTER_SCREEN( Screen );

Screen::Screen(string sName) :
	m_name(sName),
	m_projection(1.0)
{
	LOG->Info("Created screen \"%s\"", sName.c_str());

	SubscribeTo("ScreenChanged");
	SubscribeTo("Input");
}

Screen::~Screen()
{
}

void Screen::Init()
{

}

void Screen::HandleMessage(const Message &msg)
{

}

void Screen::Push(Drawable* obj)
{
	m_objects.push_back(obj);
}

void Screen::UpdateInternal(double delta)
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
 * Colby Klein, Matt Vandermeulen (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
