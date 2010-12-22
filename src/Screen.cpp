#include <GL/glfw3.h>
#include "Screen.h"
#include "ResourceManager.h"
#include "Logger.h"

void Screen::AddObject(Object* _object)
{
	m_vpObjects.push_back(_object);

	// objects are updated globally instead of per-screen. this includes deletion.
	Resources->Add(_object);
}

void Screen::AddObject(Sound* _sound)
{
	m_vpSounds.push_back(_sound);
	Resources->Add(_sound);
}

void Screen::Input(const IEvent &e)
{
	// todo
	if (e.controllers.size() > 0)
	{
		for (int i = 0; i<e.controllers[0]->num_buttons; i++)
			Log->InlinePrint("%d ", e.controllers[0]->buttons[i]);

		for (int i = 0; i<e.controllers[0]->num_axes; i++)
			Log->InlinePrint("%2.2f ", e.controllers[0]->axes[i]);
		Log->InlinePrint("\n");
	}
	if (e.keys['W'] == KEY_PRESSED)
		Log->Print("W is annoying (time: %f)", e.timestamp['W']);
}

void Screen::Update(double delta)
{
	for (unsigned i = 0; i<m_vpObjects.size(); i++)
		m_vpObjects[i]->Update(delta);

	for (unsigned i = 0; i<m_vpSounds.size(); i++)
		m_vpSounds[i]->Update(delta);
}

void Screen::Draw()
{
	// draw the objects owned by this screen.
	for (unsigned i = 0; i<m_vpObjects.size(); i++)
		m_vpObjects[i]->Draw();
}
