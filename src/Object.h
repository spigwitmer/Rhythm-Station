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

struct AnimationState
{
	AnimationState operator=(const AnimationState &anim)
	{
		tween_type = anim.tween_type;
		duration = anim.duration;
		matrix = anim.matrix;
		color = anim.color;
		active = anim.active;
		return *this;
	}

	TweenType tween_type;
	double duration;

	Matrix matrix;
	rgba color;

	bool active;
};

class Object
{
public:
	Object();
	virtual ~Object();
	void Register();

	void Load(std::string path);

	void SetParent(Object* obj);
	void AddChild(Object* obj);

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

protected:
	void QueueUpdate();

	void CreateBuffers();
	void DeleteBuffers();

	GLuint m_vbo, m_color_uniform;
	std::map<std::string, std::vector<AnimationState> > m_states;
	std::vector<Object*> m_children;

	bool m_bNeedsUpdate;
	int m_vertices;

	AnimationState m_current;
	rgba	m_color;
	Shader	m_shader;
	Matrix	m_matrix;
	Texture	m_texture;
	Timer	m_time;

	Object* m_parent;
	
	vec3 m_pos, m_rot, m_scale;
};

void Object_Binding();

#endif
