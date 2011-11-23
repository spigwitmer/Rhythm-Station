#include "Shader.h"

ShaderProgram::ShaderProgram()
{
	m_object = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	Detach();
	glDeleteProgram(m_object);
}

void ShaderProgram::Attach(ShaderStage shader)
{
	glAttachShader(m_object, shader.GetObject());
	m_shaders.push_back(shader);
}

void ShaderProgram::Detach()
{
	std::vector<ShaderStage>::iterator it;
	for (it = m_shaders.begin(); it != m_shaders.end(); it++)
		glDetachShader(m_object, it->GetObject());
	
	m_shaders.clear();
}

bool ShaderProgram::Link()
{
	glLinkProgram(m_object);
	glValidateProgram(m_object);
	
	// TODO: check log
	// return false if it's broken
	
	return true;
}

// Usage
void ShaderProgram::Bind()
{
	glUseProgram(m_object);
}
