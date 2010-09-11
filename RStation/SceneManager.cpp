#include "RStation.h"
#include "MathUtils.h"
#include "Screen.h"
#include "SceneManager.h"
#include <GL/glfw.h>
#include "Sprite.h"
#include "Timer.h"

std::vector<Screen*> vpScreens;

void Scene::PushScreen()
{
	Screen* scr = new Screen();
	vpScreens.push_back(scr);
	Log::Print("[Scene::PushScreen] Pushed a new screen to the stack.");
	
	Timer timer;
	Sprite* spr = new Sprite();
	spr->Load("Themes/rstation-logo.png");
	spr->Rotate(vec3(0,0,0));
	spr->Register();
	
	Sprite* spr_mouse = new Sprite();
	spr_mouse->Load("Themes/_arrow.png");
	spr_mouse->Hook(RS_ATTACH_CURSOR);
	int w, h;
	w = spr_mouse->getWidth();
	h = spr_mouse->getHeight();
	spr_mouse->Offset(vec3(w/13.f,h/7.f,0)); // not perfect.
	spr_mouse->Rotate(vec3(0,0,-27));
	spr_mouse->Scale(vec3(0.35,0.5,1));
	scr->AddActor(spr_mouse);
	
	Log::Print("[Scene::PushScreen] Loading took: " + timer.strAgo() + " seconds.");
}

void Scene::PopScreen()
{
	if(vpScreens.empty())
	{
		Log::Print("No Screens to delete!");
		return;
	}
	delete vpScreens.back();
	vpScreens.pop_back();

	Log::Print("[Scene::PopScreen] Deleted top screen.");
}

Screen* Scene::GetTopScreen()
{
	return vpScreens.back();
}

void Scene::Update(float deltaTime)
{
	for(unsigned i = 0; i<vpScreens.size(); i++)
		vpScreens[i]->Update(deltaTime);
}

void Scene::SendInput(const IEvent &e)
{
	if(!vpScreens.empty())
		vpScreens.back()->Input(e);
}

void Scene::Clear()
{
	while(!vpScreens.empty())
	{
		PopScreen();
	}
	Log::Print("[Scene::Clear] Cleared all screens.");
}

void Scene::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// recursively draw everything
	for(unsigned i = 0; i<vpScreens.size(); i++)
		vpScreens[i]->Draw();

	glfwSwapBuffers();
}
