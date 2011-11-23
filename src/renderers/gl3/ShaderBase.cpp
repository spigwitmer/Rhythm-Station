#include "Shader.h"
#include "Error.h"
#include "utils/Logger.h"

using namespace std;

string ShaderBase::GetInfoLog()
{
	string log;
	GLint status, count;
	GLchar *error;
	GLenum which = glIsShader(m_object) ? GL_COMPILE_STATUS : GL_LINK_STATUS;
	
	void (*GetProg)(GLuint, GLenum, GLint*);
	void (*GetProgInfo)(GLuint, GLsizei, GLsizei*, GLchar*);
	
	bool swap = (bool)glIsShader(m_object);
	GetProg = swap ? glGetShaderiv : glGetProgramiv;
	GetProgInfo = swap ? glGetShaderInfoLog : glGetProgramInfoLog;
	
	GetProg(m_object, which, &status);
	if (!status)
	{
		GetProg(m_object, GL_INFO_LOG_LENGTH, &count);
		if (count > 0)
		{
			GetProgInfo(m_object, count, NULL, (error = new char[count+1]));
			log = error;
			delete[] error;
		}
	}
	
	CheckError();
	
	return log;
}

