#ifndef _OBJ_LOADER_H_
#define _OBJ_LOADER_H_

#include "Timer.h"
#include "FileManager.h"
#include "Logger.h"
#include "RSUtil.h"
#include <GL/glew.h>
#include <vector>
#include "Type.h"

struct MeshData
{
	vec3 position, normal;
	vec2 coord;
};

class OBJLoader
{
public:
	OBJLoader();
	virtual ~OBJLoader();
	void Load(std::string file);
	void Draw();
	void Delete();

private:
	Timer timer;
	GLuint shader_id, vbo_id;
	std::vector<MeshData> mesh;

	std::string name;
};

#endif
