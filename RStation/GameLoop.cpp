#include <GL/glew.h>
#include "RStation.h"
#include <GL/glfw.h>
#include "GameLoop.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "RSUtil.h"
#include "Screen.h"
#include "Sprite.h"
#include "Sound.h"
#include "Primitives.h"
#include "RTT.h"
#include "OBJLoader.h"

bool bRunning = true;

const int freq = 2; // update x times per second

#include <math.h>
#include "MathUtils.h"

namespace Game
{
	void Terminate()
	{
		bRunning = false;
	}
	void Run()
	{
		Timer timer;
		Scene::PushScreen(); // push overlay

		Sprite *sprite = new Sprite();
		sprite->Load("Themes/rstation-logo.png");
		sprite->Glow(rgba(0.125f, 0.125f, 0.125f, 0.0f));
		sprite->Register();

		sprite = new Sprite();
		sprite->Load("Themes/_button.png");
		sprite->Saturation(-1.0);
		sprite->Position(vec3(-64,128,0));
		sprite->Register();

		sprite = new Sprite();
		sprite->Load("Themes/_button.png");
		sprite->Hue(200.0);
		sprite->Position(vec3(64,128,0));
		sprite->Register();

		// make a proper one later.
		sprite = new Sprite();
		sprite->Load("Themes/_arrow.png");
		sprite->Hook(RS_ATTACH_CURSOR);
		sprite->Offset(vec3(sprite->getWidth()/13.f,sprite->getHeight()/7.f,0));
		sprite->Rotate(vec3(0.f,0.f,-27.f));
		sprite->Scale(vec3(0.35f,0.5f,1.f));
		sprite->Register();

		Sound *sound = new Sound();
		sound->Load("Themes/shield-9.ogg");
		sound->Loop(true);
		sound->Register();

		check_gl_errors();

		Log::Print("Loading took: " + timer.strAgo() + " seconds.");
		// Init is done, flush the log.
		Log::Write();
		double then = glfwGetTime();

//		create_fbo();

		while(bRunning && glfwGetWindowParam(GLFW_OPENED))
		{
//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//			glLoadIdentity();

			// calculate delta time
			double now = glfwGetTime();
			float delta = float(now - then);

			// calculate FPS and set window title.
			if (int(then * freq) != int(now * freq))
				Util::UpdateWindowTitle(delta);

			then = now;

			// update this first so we're on the right frame
			Input::Update();

			Scene::Update(delta);

//			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, get_framebuffer());

			Scene::Draw();

//			draw_fbo();

			glfwSwapBuffers();
		}
//		delete_fbo();
		Scene::Clear();
	}
}
