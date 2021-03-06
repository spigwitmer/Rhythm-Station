#include <GL/glew.h>
#include <GL/glfw3.h>
#include <cstdlib>
#include "utils/Logger.h"
#include "Shader.h"
#include "managers/FileManager.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"

// for value_ptr
#include <glm/gtc/type_ptr.hpp>

std::string getShaderLog(GLuint obj)
{
	int infologLength = 0, charsWritten = 0;
	char *infoLog;
	std::string log;
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

std::string getProgramLog(GLuint obj)
{
	int infologLength = 0, charsWritten = 0;
	char *infoLog;
	std::string log;
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

Shader::Shader() : id(0)
{	
	/*
	 * Use the game's projection matrix just to init this with – it's
	 * not actually going to be used, it only prevents a crash.
	 */
	this->setModelViewMatrix(Game->ProjectionMatrix);
	this->setProjectionMatrix(Game->ProjectionMatrix);
	this->loadFromDisk("/Data/Shaders/generic.vs", "/Data/Shaders/generic.fs");
}

Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(id);
}

void Shader::setModelViewMatrix(Matrix *mat)
{
	m_model = mat;
}

void Shader::setProjectionMatrix(Matrix *mat)
{
	m_proj = mat;
}

void Shader::loadFromDisk(std::string _vs, std::string _fs)
{
	if (_vs.empty() || _fs.empty())
		return;
	
	_vs = FileManager::GetFile(_vs);
	_vs = FileManager::GetFileContents(_vs);
	_fs = FileManager::GetFile(_fs);
	_fs = FileManager::GetFileContents(_fs);
	this->Load(_vs, _fs);
}

void Shader::Load(const char **_vs, const char **_fs)
{
	vss = *_vs;
	fss = *_fs;
	this->Reload();
}

void Shader::Load(std::string _vs, std::string _fs)
{
	if (_vs.empty() || _fs.empty())
		return;
		
	vss = _vs.c_str();
	fss = _fs.c_str();
	this->Reload();
}

void Shader::Reload()
{
	GLuint vs, fs;
	
	// create ids for our vertex and frag shaders
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vs, 1, &vss, NULL);
	glShaderSource(fs, 1, &fss, NULL);
	
	glCompileShader(vs);
	glCompileShader(fs);
	
	// create program, attach shaders, link.
	id = glCreateProgram();
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	
	glBindAttribLocation(id, VERTEX_ARRAY, "VPos");
	glBindAttribLocation(id, NORMAL_ARRAY, "VNor");
	glBindAttribLocation(id, COORD_ARRAY, "VCoords");
	glLinkProgram(id);
	
	// We're done with these now.
	glDetachShader(id, vs);
	glDetachShader(id, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	// print out shader logs.
	std::string log = getShaderLog(vs);
	
	if (!log.empty())
		Log->Print("Vertex shader log: %s", log.c_str());
		
	log = getShaderLog(fs);
	
	if (!log.empty())
		Log->Print("Fragment shader log: %s", log.c_str());
		
	log = getProgramLog(id);
	
	if (!log.empty())
	{
		Log->Print("Shader program log: %s", log.c_str());
		// XXX
		Log->Print("catastrophic shader error. committing suicide.");
		exit(2);
	}
	
	this->Bind();
}

void Shader::Bind()
{
	if (Game->getCurrentShader() == id)
		return;

	Game->setCurrentShader(id);
	
	glUseProgram(id);
	
	setUniforms();
}

void Shader::setUniforms()
{
	// Note: m_proj is (er, should be) guaranteed to exist by GameManager.
	glUniformMatrix4fv(getUniform("ModelViewMatrix"), 1, false, glm::value_ptr(m_model->matrix));
	glUniformMatrix4fv(getUniform("ProjectionMatrix"), 1, false, glm::value_ptr(m_proj->matrix));
	
	glUniform1i(getUniform("Texture0"), 0);
}

GLuint Shader::getUniform(std::string name)
{
	if (!mUniforms[name])
		mUniforms[name] = glGetUniformLocation(id, name.c_str());

	return mUniforms[name];
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
