#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Shader.h"
#include "ResourceManager.h"
#include "Type.h"
#include "Matrix.h"
#include "Timer.h"
#include "Tween.h"
#include <vector>
#include <map>

// the fallback buffer used by sprites and quads
void GenerateQuadBuffers();
void DeleteQuadBuffers();

struct AnimationState {
	TweenType tween_type;
	double duration;

	Matrix matrix;
	rgba color;

	bool active;
};

class Object {
public:
	Object();
	virtual ~Object();
	void Register();

	void Load(std::string path);

	void Color(rgba col);
	void Translate(vec3 pos);
	void Rotate(vec3 rot);
	void Scale(vec3 scale);

	void Translate3f(float x, float y, float z) { Translate(vec3(x,y,z)); }
	void Rotate3f(float x, float y, float z) { Rotate(vec3(x,y,z)); }
	void Scale3f(float x, float y, float z) { Scale(vec3(x,y,z)); }
	void Color4f(float r, float g, float b, float a) { m_color = rgba(r,g,b,a); }

	void AddState();

	void HandleMessage(std::string msg);
	void Update(double delta);
	void Draw();

	Matrix GetMatrix() { return m_matrix; }

private:
	void QueueUpdate();

	GLuint m_vbo, m_ibo, m_color_uniform;
	std::map<std::string, std::vector<AnimationState> > m_states;

	bool m_bNeedsUpdate;

	AnimationState	m_current;
	rgba		m_color;
	Shader	m_shader;
	Matrix	m_matrix;
	Texture	m_texture;
	Timer	m_time;
	
	vec3 m_pos, m_rot, m_scale;
};

void Object_Binding();

#endif
