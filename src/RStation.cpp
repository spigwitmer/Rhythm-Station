#include "RStation.h"
#include "managers/DisplayManager.h"
#include "managers/InputManager.h"
#include "managers/LuaManager.h"
#include "managers/ScreenManager.h"
#include "utils/Logger.h"
#include <glsw.h>

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

#include <OpenGL/gl3.h>

string getShaderLog(GLuint obj)
{
	string log;
	GLint status, count;
	GLchar *error;
	
	glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &count);
		
		if (count > 0)
		{
			glGetShaderInfoLog(obj, count, NULL, (error = new char[count+1]));
			
			log = error;
			
			delete[] error;
		}
	}
	
	return log;
}

string getProgramLog(GLuint obj)
{
	string log;
	GLint status, count;
	GLchar *error;
	
	glGetProgramiv(obj, GL_LINK_STATUS, &status);
	if (!status)
	{
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &count);
		
		if (count > 0)
		{
			glGetProgramInfoLog(obj, count, NULL, (error = new char[count+1]));
			log = error;
			
			delete[] error;
		}
	}
	
	return log;
}

int RStation::Run()
{
	FileManager fileman;
	InputManager input;
	ScreenManager screen;
	DisplayManager display;
	const char *vss, *fss;
	GLuint vs, fs, id;
	string log;	
	
	// Open the display, make sure nothing went wrong on init.
	display.OpenWindow(m_window); // create context
	display.CheckError(); // perfectly fine.
	
	if (display.IsGL3())
	{
		vss = glswGetShader("Something.GL32.Vertex");
		fss = glswGetShader("Something.GL32.Fragment");
	}
	// Fall back to 2.1
	else
	{
		vss = glswGetShader("Something.GL21.Vertex");
		fss = glswGetShader("Something.GL21.Fragment");		
	}
		
	// TODO: non-test code
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
//	glBindFragDataLocation(id, 0, "FragColor");	
	glLinkProgram(id);
	display.CheckError();

	if (!(log = getShaderLog(vs)).empty()) LOG->Info("Vertex shader log: %s", log.c_str());	
	if (!(log = getShaderLog(fs)).empty()) LOG->Info("Fragment shader log: %s", log.c_str());
	if (!(log = getProgramLog(id)).empty())
	{
		LOG->Info("Shader program log: %s", log.c_str());
		LOG->Fatal("catastrophic shader error. committing suicide.");
		exit(1);
	}
	
	display.CheckError();
	
	GLuint vao;
	if (display.IsGL3()) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}
	
	glUseProgram(id);
	display.CheckError();

	float verts[] = { -1.0, -1.0, 0.0, -1.0, 1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0 };
	unsigned indices[] = { 0, 1, 2, 3 };
	
	// Identity.
	float mat[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	glUniformMatrix4fv(glGetUniformLocation(id, "ModelViewProjection"), 1, false, mat);
	
	GLuint buf[2];
	glGenBuffers(2, &buf[0]);
		
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
	display.CheckError();
	
	glEnableVertexAttribArray(0);
	display.CheckError();
	
	// Input device drivers (lua based)
	LuaManager drivers(fileman);
	drivers.Bind("/devices/");
	
	// Game scripts.
	LuaManager game(fileman);
	game.Bind("/screens/");
		
	while (true)
	{
		// Break if user closed the window
		input.Update();
		if (!glfwIsWindow(m_window) || input.GetButton(RS_KEY_ESC)->IsDown())
			break;
		
		// ScreenManager automatically calculates delta.
		screen.Update(glfwGetTime());
		screen.Draw();

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glUseProgram(id);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);

		display.CheckError();		
		display.Flush();
	}
	
	if (display.IsGL3())
		glDeleteVertexArrays(1, &vao);
	
	// Don't need these after the program is linked.
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
