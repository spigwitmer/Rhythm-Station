#include <GL/glfw3.h>
#include "globals.h"
#include "Logger.h"
#include "Shader.h"
#include "FileManager.h"
#include "ResourceManager.h"

std::string getShaderLog(GLuint obj) {
	int infologLength = 0, charsWritten = 0;
	char *infoLog;
	std::string log;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0) {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		log = infoLog;
		delete[] infoLog;
	}
	return log;
}

std::string getProgramLog(GLuint obj) {
	int infologLength = 0, charsWritten = 0;
	char *infoLog;
	std::string log;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0) {
		infoLog = new char[infologLength];
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		log = infoLog;
		delete[] infoLog;
	}
	return log;
}

Shader::Shader() {
	vs = fs = ptr = NULL;

	m_model = new Matrix();
	m_proj = new Matrix();
	this->SetProjectionMatrix(g_projection_matrix);

	const char* generic_vs[] = {
		"attribute vec4 VPos;\n"
		"attribute vec2 VCoords;\n"
		"uniform mat4 ModelViewMatrix;\n"
		"uniform mat4 ProjectionMatrix;\n"
		"\n"
		"varying vec2 Coords;\n"
		"void main() {\n"
		"	Coords = vec2(VCoords);\n"
		"	gl_Position = ProjectionMatrix * ModelViewMatrix * VPos;\n"
		"}\n"
	};

	const char* generic_fs[] = {
		"uniform sampler2D Texture0;\n"
		"uniform vec4 Color;\n"
		"varying vec2 Coords;\n"
		"\n"
		"void main() {\n"
		"	vec4 texture = texture2D(Texture0, Coords);\n"
		"	gl_FragColor = texture * vec4(1.0) * Color;\n"
		"}\n"
	};

	this->Load(generic_vs, generic_fs);
}

Shader::~Shader() {
	this->Unbind();
	glDetachShader(ptr, vs);
	glDetachShader(ptr, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);

	glDeleteProgram(ptr);
}

void Shader::SetModelViewMatrix(Matrix *mat) {
	m_model->Load(mat->matrix);
}

void Shader::SetProjectionMatrix(Matrix *mat) {
	m_proj->Load(mat->matrix);
}

void Shader::LoadFromDisk(std::string _vs, std::string _fs) {
	if (_vs.empty() || _fs.empty())
		return;

	_vs = File->GetFile(_vs);
	_vs = File->GetFileContents(_vs);

	_fs = File->GetFile(_fs);
	_fs = File->GetFileContents(_fs);

	this->Load(_vs, _fs);
}

void Shader::Load(const char** _vs, const char** _fs) {
	vss = *_vs;
	fss = *_fs;

	this->Reload();
}

void Shader::Load(std::string _vs, std::string _fs) {
	if (_vs.empty() || _fs.empty())
		return;

	vss = _vs.c_str();
	fss = _fs.c_str();

	this->Reload();
}

void Shader::Reload() {
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
	glBindAttribLocation(ptr, COORD_ARRAY, "VCoords");
//	glBindAttribLocation(ptr, NORMAL_ARRAY, "VNor");
//	glBindAttribLocation(ptr, COLOR_ARRAY, "VCol");

	glLinkProgram(ptr);

	// print out shader logs.
	std::string log = getShaderLog(vs);
	if (!log.empty()) Log->Print("Vertex shader log: " + log);
	log = getShaderLog(fs);
	if (!log.empty()) Log->Print("Fragment shader log: " + log);
	log = getProgramLog(ptr);
	if (!log.empty()) {
		Log->Print("Shader program log: " + log);
		Log->Print("catastrophic shader error. committing suicide.");
		exit(-1);
	}

	this->Bind();

	m_mv_uniform = glGetUniformLocation(ptr, "ModelViewMatrix");
	m_proj_uniform = glGetUniformLocation(ptr, "ProjectionMatrix");
	m_tex_uniform = glGetUniformLocation(ptr, "Texture0");
}

void Shader::Bind() {
	glUseProgram(ptr);
}

void Shader::SetUniforms() {
	glUniformMatrix4fv(m_mv_uniform, 1, false, m_model->matrix);
	glUniformMatrix4fv(m_proj_uniform, 1, false, m_proj->matrix);
	glUniform1i(m_tex_uniform, 0);
}

void Shader::Unbind() {
	glUseProgram(0);
}
