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
		{
			Sprite *spr = new Sprite();
			spr->Load("Themes/rstation-logo.png");
			spr->Glow(rgba(0.25f, 0.25f, 0.25f, 0.0f));
			spr->Register();

			Sprite *spr_mouse = new Sprite();
			spr_mouse->Load("Themes/_arrow.png");
			spr_mouse->Hook(RS_ATTACH_CURSOR);
			int w = spr_mouse->getWidth();
			int h = spr_mouse->getHeight();
			spr_mouse->Offset(vec3(w/13.f,h/7.f,0)); // not perfect.
			spr_mouse->Rotate(vec3(0.f,0.f,-27.f));
			spr_mouse->Scale(vec3(0.35f,0.5f,1.f));
			spr_mouse->Register();

			Sound *sound = new Sound();
			sound->Load("Themes/shield-9.ogg");
			sound->Loop(true);
			sound->Register();
		}
		
		Log::Print("Loading took: " + timer.strAgo() + " seconds.");
		// Init is done, flush the log.
		Log::Write();
		ShaderLoader* post = new ShaderLoader();
		post->Load("sprite.vert","nothing.frag");
		double then = glfwGetTime();
		create_fbo();
		while(bRunning && glfwGetWindowParam(GLFW_OPENED))
		{
			// calculate delta time
			double now = glfwGetTime();
			float delta = float(now - then);

			// calculate FPS and set window title.
			if( int(then * freq) != int(now * freq) )
				Util::UpdateWindowTitle(delta);

			then = now;

			// update this first so we're on the right frame
			Input::Update();

			Scene::Update(delta);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, get_framebuffer());

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			int hw = 854/2;
			int hh = 480/2;

			glOrtho(-hw, hw, hh, -hh, -hw, hw);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			
			Scene::Draw();
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // return to back buffer
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, get_framebuffer_tex());
			post->Bind();
			glDisable(GL_CULL_FACE);
			glPushMatrix();
				glScalef(1.0, -1.0, 1.0);
				Primitive::Quad(vec2(854,480));
			glPopMatrix();
			post->Unbind();
			glEnable(GL_CULL_FACE);
			glBindTexture(GL_TEXTURE_2D, 0);
			glfwSwapBuffers();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		delete post;
		Scene::Clear();
	}
}
