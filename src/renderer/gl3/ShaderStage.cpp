#include "Shader.h"
#include <glsw.h>
#include <map>
#include "utils/Logger.h"
#include "renderer/common/Error.h"

using namespace std;

// cache shaders which have already been built.
namespace
{
	// map of opengl-generated shader id's
	map<GLuint, int> g_shaderHandles;
	typedef map<GLuint, int>::iterator shaderHandle_iterator;

	// map of previously loaded shader stages.
	map<string, ShaderStage> g_shaderNames;
	typedef map<string, ShaderStage>::iterator shaderName_iterator;
}

ShaderStage::ShaderStage() :
	m_cached(false)
{
}

ShaderStage::~ShaderStage()
{
	// If the shader was never loaded.
	if (!glIsShader(m_object)) {
		LOG->Warn("Tried to delete a deleted shader!");
		return;
	}

	if (g_shaderHandles[m_object] <= 0)
	{
		glDeleteShader(m_object);

		shaderHandle_iterator it = g_shaderHandles.find(m_object);
		//shaderName_iterator it2 = g_shaderNames.find(m_name);

		if (it != g_shaderHandles.end())
			g_shaderHandles.erase(it);

//		if (it2 != g_shaderNames.end())
//			g_shaderNames.erase(it2);
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

bool ShaderStage::Load(ShaderType type, string path)
{
	const char *src = glswGetShader(path.c_str());

	LOG->Info("Loading \"%s\"", path.c_str());

	LoadInternal(type, src, path);

	if (Compile())
	{
		LOG->Info("Successfully loaded.", path.c_str());
		return true;
	}

	return false;
}

// Use const char* internally to minimize pointless conversions.
void ShaderStage::LoadInternal(ShaderType type, const char *source, string name)
{
	// FIXME
#if 0
	if (g_shaderNames.find(name) != g_shaderNames.end())
	{
		ShaderStage replace = g_shaderNames[name];
		m_cached = true;
		m_name = replace.m_name;
		m_object = replace.m_object;
		m_type = replace.m_type;

		g_shaderHandles[m_object]++;

		return;
	}
#endif
	m_name = name;

	m_object = glCreateShader(ShaderToGLenum(type));
	g_shaderHandles[m_object]++;

	glShaderSource(m_object, 1, &source, NULL);
}

bool ShaderStage::Compile()
{
	// Already have a copy - no need to rebuild.
	if (m_cached)
		return true;

	glCompileShader(m_object);

	string log = GetInfoLog();
	if (!log.empty())
		LOG->Info(log);

	CheckError();

	// Compile went fine, register self.
	if (glIsShader(m_object)) {
//		g_shaderNames[m_name] = *this;
		return true;
	}

	// bad mojo.
	return false;
}
