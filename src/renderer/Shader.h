#ifndef _SHADER_H_
#define _SHADER_H_

class Shader
{
public:
	Shader();
	virtual ~Shader();

	void Bind();
	
	void AddUniform(int, float);
	void AddUniform(int, float*);
	void AddUniform(int, int);
	void AddUniform(int, int*);
};

#endif

