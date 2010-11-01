#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Shader.h"
#include "ResourceManager.h"
#include "Type.h"
#include "Matrix.h"
#include "Timer.h"
#include <vector>
#include <map>

// the fallback buffer used by sprites and quads
void GenerateQuadBuffers();
void DeleteQuadBuffers();

struct AnimationState {
	Matrix matrix;
	rgba color;
	bool active;
};

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

	void AddState();

	void HandleMessage(std::string msg);
	void Update(double delta);
	void Draw();

	Matrix GetMatrix() { return m_matrix; }

private:
	GLuint m_vbo, m_ibo, m_color_uniform;
	std::map<std::string, std::vector<AnimationState> > m_states;

	AnimationState m_current;
	rgba m_color;
	Shader m_shader;
	Matrix m_matrix;
	Texture m_texture;
	Timer m_timer;
};

#endif
