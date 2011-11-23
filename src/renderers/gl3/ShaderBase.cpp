#include "Shader.h"
#include "DisplayManager.h"

using namespace std;

DisplayManager display;

string ShaderBase::GetInfoLog(GLuint obj)
{
	string log;
	GLint status, count;
	GLchar *error;
	GLenum which = glIsShader(obj) ? GL_COMPILE_STATUS : GL_LINK_STATUS;
	
	void (*GetProg)(GLuint, GLenum, GLint*);
	void (*GetProgInfo)(GLuint, GLsizei, GLsizei*, GLchar*);
	
	bool swap = (bool)glIsShader(obj);
	GetProg = swap ? glGetShaderiv : glGetProgramiv;
	GetProgInfo = swap ? glGetShaderInfoLog : glGetProgramInfoLog;
	
	GetProg(obj, which, &status);
	if (!status)
	{
		GetProg(obj, GL_INFO_LOG_LENGTH, &count);
		if (count > 0)
		{
			GetProgInfo(obj, count, NULL, (error = new char[count+1]));
			log = error;
			delete[] error;
		}
	}
	
	display.CheckError();
	
	return log;
}

