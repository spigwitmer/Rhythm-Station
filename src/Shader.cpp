#include <GL/glew.h>
#include <GL/glfw3.h>
#include <cstdlib>
#include "Logger.h"
#include "Shader.h"
#include "FileManager.h"
#include "ResourceManager.h"
#include "GameManager.h"

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

Shader::Shader()
{
	vs = fs = ptr = 0;

	m_model = new Matrix();
	m_proj = new Matrix();

	this->SetProjectionMatrix(Game->ProjectionMatrix);
	this->LoadFromDisk("Data/Shaders/generic.vs", "Data/Shaders/generic.fs");
}

Shader::~Shader()
{
	this->Unbind();

	glDetachShader(ptr, vs);
	glDetachShader(ptr, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);

	glDeleteProgram(ptr);
}

void Shader::SetModelViewMatrix(Matrix *mat)
{
	m_model = mat;
}

void Shader::SetProjectionMatrix(Matrix *mat)
{
	m_proj = mat;
}

void Shader::LoadFromDisk(std::string _vs, std::string _fs)
{
	if (_vs.empty() || _fs.empty())
		return;

	_vs = File->GetFile(_vs);
	_vs = File->GetFileContents(_vs);

	_fs = File->GetFile(_fs);
	_fs = File->GetFileContents(_fs);

	this->Load(_vs, _fs);
}

void Shader::Load(const char** _vs, const char** _fs)
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
	// create pointers for our vertex and frag shaders
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vss, NULL);
	glShaderSource(fs, 1, &fss, NULL);

	glCompileShader(vs);
	glCompileShader(fs);

	// create program, attach shaders, link.
	ptr = glCreateProgram();
	glAttachShader(ptr, vs);
	glAttachShader(ptr, fs);

	glBindAttribLocation(ptr, VERTEX_ARRAY, "VPos");
	glBindAttribLocation(ptr, NORMAL_ARRAY, "VNor");
	glBindAttribLocation(ptr, COORD_ARRAY, "VCoords");

	glLinkProgram(ptr);

	// print out shader logs.
	std::string log = getShaderLog(vs);
	if (!log.empty())
		Log->Print("Vertex shader log: %s", log.c_str());

	log = getShaderLog(fs);
	if (!log.empty())
		Log->Print("Fragment shader log: %s", log.c_str());

	log = getProgramLog(ptr);
	if (!log.empty())
	{
		Log->Print("Shader program log: %s", log.c_str());

		// XXX
		Log->Print("catastrophic shader error. committing suicide.");
		exit(2);
	}

	this->Bind();

	m_mv_uniform = glGetUniformLocation(ptr, "ModelViewMatrix");
	m_proj_uniform = glGetUniformLocation(ptr, "ProjectionMatrix");
	m_tex_uniform = glGetUniformLocation(ptr, "Texture0");
}

void Shader::Bind()
{
	if (ptr != Game->GetCurrentShader())
	{
		glUseProgram(ptr);
		Game->SetCurrentShader(ptr);
	}
}

void Shader::SetUniforms()
{
	glUniformMatrix4fv(m_mv_uniform, 1, false, m_model->matrix);
	glUniformMatrix4fv(m_proj_uniform, 1, false, m_proj->matrix);
	glUniform1i(m_tex_uniform, 0);
}

void Shader::Unbind()
{
	glUseProgram(0);
}
