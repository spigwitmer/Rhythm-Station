#include "Screen.h"
#include "Object.h"
#include "SceneManager.h"
#include "Logger.h"

SceneManager* Scene = NULL;

// refactor
void SceneManager::PushScreen() {
	Screen* scr = new Screen();
	vpScreens.push_back(scr);
	Log->Print("Pushed a new screen to the stack.");	
}

void SceneManager::PopScreen() {
	if (vpScreens.empty()) {
		Log->Print("No Screens to delete!");
		return;
	}
	delete vpScreens.back();
	vpScreens.pop_back();

	Log->Print("Deleted top screen.");
}

Screen* SceneManager::GetTopScreen() {
	return vpScreens.back();
}

void SceneManager::Update(float deltaTime) {
	for (unsigned i = 0; i<vpScreens.size(); i++)
		vpScreens[i]->Update(deltaTime);
}

/*
void SceneManager::SendInput(IEvent &e) {
	if (!vpScreens.empty())
		vpScreens.back()->Input(e);
}
*/

void SceneManager::Clear() {
	while (!vpScreens.empty())
		PopScreen();
	Log->Print("Cleared all screens.");
}

void SceneManager::Draw() {
	// draw all screens and their children
	for (unsigned i = 0; i<vpScreens.size(); i++)
		vpScreens[i]->Draw();
}
