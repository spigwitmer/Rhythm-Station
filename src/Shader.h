#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/glfw3.h>
#include <string>
#include <map>
#include "Matrix.h"

enum
{
	VERTEX_ARRAY = 0,
	NORMAL_ARRAY,
	COORD_ARRAY
};

class Shader
{
public:
	Shader();
	virtual ~Shader();
	
	void Load(const char **vs, const char **fs);
	void Load(std::string vs, std::string fs);
	void loadFromDisk(std::string vs, std::string fs);
	void Reload();
	
	void setModelViewMatrix(Matrix *mat);
	void setProjectionMatrix(Matrix *mat);
	void setUniforms();
	
	GLuint getUniform(std::string name);
	
	void Bind();
	
	std::string path[2];	// paths for dupe checking.

	GLuint id;
	
	Shader operator= (const Shader&) { return *this; }
	
private:
	std::map<std::string, GLuint> mUniforms;
	const char *vss, *fss;	// shader sources.
	Matrix *m_proj, *m_model;
};

#endif
