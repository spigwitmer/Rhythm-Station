#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Shader.h"
#include "Type.h"
#include "Matrix.h"
#include <vector>
#include <map>

// the fallback buffer used by sprites and quads
void GenerateQuadBuffers();
void DeleteQuadBuffers();

class Object {
public:
	Object();
	virtual ~Object();
	void Register();

	// TODO: automatically work out models, images.
	void Load(std::string path);

	void Color(rgba col);

	void Translate(vec3 pos);
	void Rotate(vec3 rot);
	void Scale(vec3 scale);

	void HandleMessage(std::string msg);
	void Update(double delta);
	void Draw();

private:
	GLuint m_vbo, m_ibo, m_texture, m_color_uniform;
	std::map<std::string, std::vector<Matrix> > m_states;

	rgba m_color;
	Shader m_shader;
	Matrix m_mat;
};

#endif
