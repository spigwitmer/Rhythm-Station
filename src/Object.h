#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Shader.h"
#include "Message.h"
#include "ResourceManager.h"
#include "Type.h"
#include "Mesh.h"
#include "Matrix.h"
#include "Timer.h"
#include "Tween.h"
#include <vector>
#include <map>

struct AnimationState
{
	AnimationState() : type(TWEEN_SLEEP), duration(0.0), color(rgba(1.0)),
	position(vec3(0.0)), rotation(vec3(0.0)), scale(vec3(1.0))
	{ }
	AnimationState operator = (const AnimationState &anim)
	{
		type = anim.type;
		duration = anim.duration;
		color = anim.color;
		position = anim.position;
		rotation = anim.rotation;
		scale = anim.scale;
		return *this;
	}
	
	TweenType type;
	double duration;
	
	rgba color;
	vec3 position, rotation, scale;
};

class Object : public MessageListener
{
public:
	Object();
	virtual ~Object();
	
	void Load(std::string path);
	
	void setParent(Object *obj);
	void addChild(Object *obj);
	
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setColor(float r, float g, float b, float a);
	
	void addState(int tweentype, double length);
	
	void HandleMessage(std::string msg);
	void Update(double delta);
	void Draw();
	
	Matrix GetMatrix() {
		return m_matrix;
	}
	
	void DepthClear(bool enabled);
	
	vec3 getTranslation() {
		return m_pos;
	}
	vec3 getRotation() {
		return m_rot;
	}
	vec3 getScale() {
		return m_scale;
	}
	
	
protected:
	void Register();
	
	void QueueUpdate();
	
	Mesh mesh;
	GLuint m_color_uniform;
	
	std::vector<AnimationState> m_states;
	std::vector<Object*> m_children;
	
	bool m_bNeedsUpdate, m_bDepthClear;
	
	unsigned m_frame;
	
	rgba	m_color;
	Shader	m_shader;
	Matrix	m_matrix;
	Texture	m_texture;
	Timer	m_timer;
	
	Object *m_parent;
	
	vec3 m_pos, m_rot, m_scale;
};

void Object_Binding();

#endif
