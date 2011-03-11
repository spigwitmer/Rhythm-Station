#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/glfw3.h>
#include <string>
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
	void LoadFromDisk(std::string vs, std::string fs);
	void Reload();
	
	void SetModelViewMatrix(Matrix *mat);
	void SetProjectionMatrix(Matrix *mat);
	void SetUniforms();
	
	void Bind();
	void Unbind();
	
	std::string path[2];	// paths for dupe checking.
	
	GLuint id;
		
private:
	const char *vss, *fss;	// shader sources.
	Matrix *m_proj, *m_model;
};

#endif
