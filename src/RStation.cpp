#include <GL3/gl3w.h>
#include <glsw.h>
#include "RStation.h"
#include "managers/DisplayManager.h"
#include "managers/InputManager.h"
#include "managers/LuaManager.h"
#include "managers/ScreenManager.h"
#include "utils/Logger.h"
#include <deque>
#include <glm/glm.hpp>

using namespace std;

RStation::RStation(std::vector<std::string> &vArgs)
{
	int err = glfwInit();
	m_vArgs = vArgs;
	LOG = new Logger();
	if (!err)
		LOG->Fatal(string(glfwErrorString(err)));
}

RStation::~RStation()
{
	SAFE_DELETE(LOG);
}

int RStation::Run()
{
	FileManager fileman;
	InputManager input;
	ScreenManager screen;
	DisplayManager display;
	
	const char *vss, *fss;
	GLuint vs, fs, id, vao, buf[2];
	string log;	
	
	// Open the display, make sure nothing went wrong on init.
	if (!display.OpenWindow())
		return 1;
	
	vss = glswGetShader("Generic.GL32.Vertex");
	fss = glswGetShader("Generic.GL32.Fragment");
	
	// TODO: non-test code
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, &buf[0]);

	id = glCreateProgram();
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vs, 1, &vss, NULL);
	glShaderSource(fs, 1, &fss, NULL);
	
	glCompileShader(vs);
	glCompileShader(fs);
	
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	
	glBindAttribLocation(id, 0, "Position");
	glLinkProgram(id);
	display.CheckError();
	
	if (!(log = display.GetInfoLog(vs)).empty()) LOG->Info("Vertex shader log: %s", log.c_str());	
	if (!(log = display.GetInfoLog(fs)).empty()) LOG->Info("Fragment shader log: %s", log.c_str());
	if (!(log = display.GetInfoLog(id)).empty()) LOG->Fatal("Shader program log: %s", log.c_str());
	
	glBindVertexArray(vao);
	
	glUseProgram(id);
	display.CheckError();
	
	float verts[] = { -1.0, -1.0, 0.0, -1.0, 1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0 };
	unsigned indices[] = { 0, 1, 2, 3 };
	
	// Identity.
	float mat[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	glUniformMatrix4fv(glGetUniformLocation(id, "ModelViewProjection"), 1, false, mat);
	
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);	
	glEnableVertexAttribArray(0);
	display.CheckError();
	
	// Input device drivers (lua based)
	LuaManager drivers(fileman);
	drivers.Bind("/devices/");
	
	// Game scripts.
	LuaManager game(fileman);
	game.Bind("/screens/");
	
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_1D, tex);
	unsigned data[] = {
		0x00000011,
		0x11111111,
		0x22222211,
		0x33333311,
		0x44444411,
		0x55555511,
		0x66666611,
		0x77777711,
		0x88888811,
		0x99999911,
		0xAAAAAA11,
		0xBBBBBB11,
		0xCCCCCC11,
		0xDDDDDD11,
		0xEEEEEE11,
		0xFFFFFF11,
	};
	
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, sizeof(data), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glUniform1i(glGetUniformLocation(id, "ColorTable"), 1);
	display.CheckError();
	
	int last_update = 0;
	double then = glfwGetTime(), now = 0.0;
	std::deque<double> times;
	while (true)
	{
		double delta = 0.0;
		now = glfwGetTime();
		delta = now - then;
		
		glUniform1f(glGetUniformLocation(id, "Time"), now);
		
		{
			// Keep 5 seconds worth of deltas (@60fps)
			const size_t NUM_FRAMES = 60*5;
			double avg = 0.0;
			
			// Calculate Average FPS.
			times.push_back(delta);
			
			if (times.size() > NUM_FRAMES)
				times.pop_front();
			
			for (std::deque<double>::iterator i = times.begin(); i != times.end(); i++)
				avg += *i;
			
			if (int(now) % 1 == 0 && avg > 0.0001)
			{
				if (last_update != int(now))
					LOG->Info("Avg. FPS: %0.0f",
					  glm::ceil(1.0 / (avg / times.size())));
				last_update = int(now);
			}
		}

		then = now;
		// Break if user closed the window
		input.Update();

		if (!glfwIsWindow(display.GetWindow()) || input.GetButton(RS_KEY_ESC)->IsDown())
			break;
		
		// ScreenManager automatically calculates delta.
		screen.Update(glfwGetTime());
		screen.Draw();
		
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
		
		if (!display.IsFocused())
			usleep(50000);
		
		display.CheckError();
		display.Flush();
	}
	
	display.CloseWindow();
	
	glDeleteVertexArrays(1, &vao);
	
	glDetachShader(id, vs);
	glDetachShader(id, fs);
	
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	glDeleteProgram(id);
	glDeleteBuffers(2, buf);
	
	return 0;
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
