#include "Screen.h"
#include "ResourceManager.h"
#include "Logger.h"

Screen::Screen() { }

Screen::~Screen() { }

void Screen::AddObject(Object* _object) {
	m_vpObjects.push_back(_object);

	// objects are updated globally instead of per-screen. this includes deletion.
	Resources->Add(_object);
}

void Screen::AddObject(Sound* _sound) {
	m_vpSounds.push_back(_sound);
	Resources->Add(_sound);
}

void Screen::Input(const IEvent &e) {
	// todo
	Log->Print("Recieved Input");
}

void Screen::Update(float deltaTime) {
	// objects are updated by gamemanager
}

void Screen::Draw() {
	// draw the objects owned by this screen.
	for (unsigned i = 0; i<m_vpObjects.size(); i++)
		m_vpObjects[i]->Draw();
}
