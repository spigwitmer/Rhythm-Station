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
	m_vArgs = vArgs;

	LOG = new Logger();
	int err = glfwInit();
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
	int infologLength = 0, charsWritten = 0;
	char *infoLog;
	string log;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	
	if (infologLength > 0)
	{
		infoLog = new char[infologLength];
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		log = infoLog;
		delete[] infoLog;
	}
	
	return log;
}

string getProgramLog(GLuint obj)
{
	int infologLength = 0, charsWritten = 0;
	char *infoLog;
	string log;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	
	if (infologLength > 0)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		log = infoLog;
		delete[] infoLog;
	}
	
	return log;
}

int RStation::Run()
{
	FileManager fileman;
	InputManager input;
	ScreenManager screen;
	DisplayManager display;
	
	/*
	char buf[1024];
	getcwd(buf, 1024);

	const char* shader = glswGetShader("Something.GL32.Vertex");
	if (shader == NULL) 
		LOG->Info("(%s) %s", buf, glswGetError());
	else
		LOG->Info("%s", shader);
	*/
		
//	fileman.Mount("/", string(buf));
	
	// Open the display, make sure nothing went wrong on init.
	display.OpenWindow(m_window);
	display.CheckError();
	
	GLuint vs, fs, id;
	id = glCreateProgram();
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	//	gs = glCreateShader(GL_GEOMETRY_SHADER);
	
	const char *vss, *fss;
	vss = glswGetShader("Something.GL32.Vertex");
	fss = glswGetShader("Something.GL32.Fragment");
	
	glShaderSource(vs, 1, &vss, NULL);
	glShaderSource(fs, 1, &fss, NULL);
	
	glCompileShader(vs);
	glCompileShader(fs);
	
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	
	glBindAttribLocation(id, 0, "Position");
	
	glLinkProgram(id);
	
	// We're done with these now.
	glDetachShader(id, vs);
	glDetachShader(id, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	// print out shader logs.
	string log = getShaderLog(vs);
	
	if (!log.empty())
		LOG->Info("Vertex shader log: %s", log.c_str());
	
	log = getShaderLog(fs);
	
	if (!log.empty())
		LOG->Info("Fragment shader log: %s", log.c_str());
	
	log = getProgramLog(id);
	
	if (!log.empty())
	{
		LOG->Info("Shader program log: %s", log.c_str());
		LOG->Fatal("catastrophic shader error. committing suicide.");
		exit(1);
	}
	
	glUseProgram(id);
	
	// test is over, get your shit and get out.
	glDeleteProgram(id);
	
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
		
		display.Flush();
	}
	
	return 0;
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
