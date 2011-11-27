#pragma once

#include <OpenGL/gl3.h>
#include <vector>
#include <string>

enum ShaderType {
	SHADER_VERTEX = 0,
	SHADER_FRAGMENT,
	SHADER_GEOMETRY,
	SHADER_INVALID
};

class ShaderBase
{
public:
	std::string GetName();
	void SetName();

	std::string GetInfoLog();

	GLuint GetObject() const { return m_object; }

protected:
	std::string m_name;
	GLuint m_object;
};

// Used in construction and destruction of Programs.
class ShaderStage : public ShaderBase
{
public:
	ShaderStage();
	virtual ~ShaderStage();
	
	bool Load(ShaderType type, std::string key);
	void LoadString(ShaderType, std::string source, std::string name = "");
	
	ShaderType GetType() const { return m_type; }

protected:
	void LoadInternal(ShaderType type, const char *source, std::string name);
	bool Compile();

	bool m_cached;
	ShaderType m_type;
};

class ShaderProgram : public ShaderBase
{
public:
	virtual ~ShaderProgram();

	void Create();
	
	// Setup
	void Attach(ShaderStage shader);
	void Detach();
	bool Link();
	
	// Usage
	void Bind();

protected:
	std::vector<ShaderStage> m_shaders;
};
