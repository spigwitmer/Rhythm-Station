#include <GL/glew.h>
#include "RStation.h"
#include <GL/glfw.h>
#include "GameLoop.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "RSUtil.h"
#include "Screen.h"
#include "Sprite.h"

bool bRunning = true;
const int freq = 2; // update x times per second

namespace Game
{
	void Terminate() { bRunning = false; }
	void Run()
	{
		Scene::PushScreen();

		Sprite *sprite = new Sprite();
		sprite->Load("Themes/_button.png");
		sprite->Hue(200.0);
		sprite->Position(vec3(0,210,0));
		sprite->Register();

		// make a proper one later.
		sprite = new Sprite();
		sprite->Load("Themes/_arrow.png");
		sprite->Hook(RS_ATTACH_CURSOR);
		sprite->Offset(vec3(sprite->getWidth()/13.f,sprite->getHeight()/7.f,0));
		sprite->Rotate(vec3(0.f,0.f,-27.f));
		sprite->Scale(vec3(0.35f,0.5f,1.f));
		sprite->Register();

		Log::Write();
		double then = glfwGetTime();
		while (bRunning && glfwGetWindowParam(GLFW_OPENED))
		{
			double now = glfwGetTime();
			float delta = float(now - then);

			// update window title
			if (int(then * freq) != int(now * freq))
				Util::UpdateWindowTitle(delta);

			then = now;

			// update this first so we're on the right frame
			Input::Update();
			Scene::Update(delta);

			Scene::Draw();
		}
		Scene::Clear();
	}
}
