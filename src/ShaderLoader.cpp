#include <GL/glew.h> // must be before glfw.h!
#include "ShaderLoader.h"
#include "ShaderManager.h"
#include "FileManager.h"

std::string getShaderLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	std::string log;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

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
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	std::string log;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		log = infoLog;
		delete[] infoLog;
	}
	return log;
}

ShaderLoader::ShaderLoader()
{
	shader.ptr = NULL;
}

ShaderLoader::~ShaderLoader()
{
	this->Unbind();
	this->Unload();
}

void ShaderLoader::Load(std::string _vs, std::string _fs, bool reload)
{
	if (reload)
		this->Unload(); // unload everything if reloading.

	shader.path = _vs + "+" + _fs;
	Shader dupe = ShaderManager::CheckForDuplicates(shader.path);

	if (dupe.ptr)
	{
		this->shader = dupe;
		pAspect = glGetUniformLocation(shader.ptr, "aspect_ratio");
		this->Bind();
		return;
	}

	_vs = "GameData/Shaders/" + _vs;
	_fs = "GameData/Shaders/" + _fs;

	// expand the file paths for vs and fs.
	_vs = File->GetFile(_vs);
	_fs = File->GetFile(_fs);

	// load up the files
	_vs = File->GetFileContents(_vs);
	_fs = File->GetFileContents(_fs);

	// bad paths!
	if (_vs.size() == 0 || _fs.size() == 0)
		return;

	// create pointers for our vertex and frag shaders
	shader.vs = glCreateShader(GL_VERTEX_SHADER);
	shader.fs = glCreateShader(GL_FRAGMENT_SHADER);	

	const char *vss = _vs.c_str();
	const char *fss = _fs.c_str();

	// set the shader sources
	glShaderSource(shader.vs, 1, &vss, NULL);
	glShaderSource(shader.fs, 1, &fss, NULL);

	// compile shaders
	glCompileShader(shader.vs);
	glCompileShader(shader.fs);

	// create program, attach shaders, link.
	shader.ptr = glCreateProgram();

	glAttachShader(shader.ptr, shader.vs);
	glAttachShader(shader.ptr, shader.fs);

	glLinkProgram(shader.ptr);

	// print out shader logs.
	std::string log = getShaderLog(shader.vs);
	if (!log.empty())
		Log->Print("Vertex shader log: " + log);
	log = getShaderLog(shader.fs);
	if (!log.empty())
		Log->Print("Fragment shader log: " + log);
	log = getProgramLog(shader.ptr);
	if (!log.empty())
		Log->Print("Shader program log: " + log);

	glUseProgram(shader.ptr);
	ShaderManager::addShader(shader);
	pAspect = glGetUniformLocation(shader.ptr, "aspect_ratio");
}

void ShaderLoader::Load(GLuint _program)
{
	if (shader.ptr == _program)
		return;
	shader.ptr = _program;
	glUseProgram(shader.ptr);
	pAspect = glGetUniformLocation(shader.ptr, "aspect_ratio");
}

void ShaderLoader::Unload()
{
	// detach shaders from the program so they can be deleted
	glDetachShader(shader.ptr, shader.vs);
	glDetachShader(shader.ptr, shader.fs);

	// delete shaders
	glDeleteShader(shader.fs);
	glDeleteShader(shader.vs);

	// finally, delete the program.
	glDeleteProgram(shader.ptr);

	// ready to reload
	shader.path = "";
	shader.ptr = NULL;
}

void ShaderLoader::Bind()
{
	if (shader.ptr)
		glUseProgram(shader.ptr);
	glUniform1f(pAspect, g_aspectRatio);
}

void ShaderLoader::Unbind()
{
	glUseProgram(0);
}