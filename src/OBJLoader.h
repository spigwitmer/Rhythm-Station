#ifndef _OBJ_LOADER_H_
#define _OBJ_LOADER_H_

#include "Timer.h"
#include "FileManager.h"
#include "StringUtils.h"
#include <GL/glew.h>
#include <vector>
#include "Type.h"

struct MeshData {
	vec3 position, normal;
	vec2 coord;
};

class OBJLoader {
public:
	OBJLoader();
	virtual ~OBJLoader();
	GLuint *Load(std::string file);

private:
	Timer timer;
	GLuint vbo_id[2];
	std::vector<MeshData> mesh;

	std::string name;
};

#endif
