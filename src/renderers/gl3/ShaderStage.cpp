#include "Shader.h"
#include <glsw.h>
#include <map>
#include "utils/Logger.h"
#include "Error.h"

using namespace std;

// Store shader ID's an number of users.
namespace
{
	map<GLuint, int> g_shaders;
	typedef map<GLuint, int>::iterator g_shaders_iterator;
}

ShaderStage::~ShaderStage()
{
	// If the shader was never loaded.
	if (!glIsShader(m_object)) {
		LOG->Warn("Tried to delete a deleted shader!");
		return;
	}

	if (g_shaders[m_object] <= 0)
	{
		glDeleteShader(m_object);
		
		g_shaders_iterator it;
		it = g_shaders.find(m_object);
		
		if (it != g_shaders.end())
			g_shaders.erase(it);
	}
}

namespace
{
	GLenum ShaderToGLenum(ShaderType shader)
	{
		GLenum ret;
		switch (shader)
		{
			case SHADER_VERTEX:
				ret = GL_VERTEX_SHADER;
				break;
			case SHADER_FRAGMENT:
				ret = GL_FRAGMENT_SHADER;
				break;
			case SHADER_GEOMETRY:
				ret = GL_GEOMETRY_SHADER;
				break;
			case SHADER_INVALID:
			default:
				ret = (GLenum)-1;
				break;
		}
		return ret;
	}
}

void ShaderStage::LoadString(ShaderType type, string source, string name)
{
	LoadInternal(type, source.c_str(), name);
}

void ShaderStage::Load(ShaderType type, string path, string name)
{
	const char *src = glswGetShader(path.c_str());
	LoadInternal(type, src, name);
}

// Use const char* internally to minimize pointless conversions.
void ShaderStage::LoadInternal(ShaderType type, const char *source, string name)
{
	m_name = name;

	m_object = glCreateShader(ShaderToGLenum(type));
	g_shaders[m_object]++;

	glShaderSource(m_object, 1, &source, NULL);
}

bool ShaderStage::Compile()
{
	glCompileShader(m_object);

	string log = GetInfoLog();
	if (!log.empty())
		LOG->Info(GetInfoLog());

	CheckError();
	
	return glIsShader(m_object);
}

ShaderType ShaderStage::GetType()
{
	return m_type;
}
