#include "Shader.h"
#include "utils/Logger.h"
#include "renderer/common/Error.h"

using namespace std;

ShaderProgram::~ShaderProgram()
{
	if (glIsProgram(m_object))
		glDeleteProgram(m_object);
}

void ShaderProgram::Attach(ShaderStage shader)
{
	if (!glIsProgram(m_object))
		m_object = glCreateProgram();
	
	glAttachShader(m_object, shader.GetObject());
	CheckError();
}

bool ShaderProgram::Link()
{
	glLinkProgram(m_object);
	glValidateProgram(m_object);
	
	string log = GetInfoLog();
	if (!log.empty()) {
		LOG->Warn("Shader program link failed: %s", log.c_str());
		return false;
	}
	
	return true;
}

// Usage
void ShaderProgram::Bind()
{
	glUseProgram(m_object);
}
