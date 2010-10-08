#ifndef _SHADER_H_
#define _SHADER_H_

#include "RStation.h"
#include <GL/glfw.h>
#include "ShaderManager.h"

// TODO: support uniforms, more functions, etc.
class ShaderLoader
{
public:
	ShaderLoader();
	virtual ~ShaderLoader();

	/*
	 * Load up the vs and fs in one go, overload so we can use another program.
	 * This is useful so that we can use another program and just store different
	 * uniforms for it, without having to worry about it making all our code elsewhere
	 * a big mess of swapping junk.
	 */
	void Load(std::string _vs, std::string _fs, bool reload = false);
	void Load(GLuint _program);
	void Unload();

	void Bind();
	void Unbind();

	// if we would like to store this elsewhere.
	GLuint getProgram() { return shader.ptr; }

private:
	GLuint pAspect;
	Shader shader;
};

#endif
