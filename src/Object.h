#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <vector>
#include <map>

#include "Matrix.h"
#include "Message.h"
#include "Timer.h"
#include "Tween.h"
#include "Type.h"
#include "Shader.h"
#include "Texture.h"

struct AnimationState
{
	// Make sure things don't go uninitialized!
	AnimationState() : type(TWEEN_SLEEP), duration(0.0), color(rgba(1.0)),
	position(vec3(0.0)), rotation(vec3(0.0)), scale(vec3(1.0)) { }
	
	AnimationState operator = (const AnimationState &anim)
	{
		type = anim.type;
		duration = anim.duration;
		position = anim.position;
		rotation = anim.rotation;
		scale = anim.scale;
		color = anim.color;
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
	
	virtual void Load(std::string path) { }
	
	void HandleMessage(std::string msg);
	void Update(double delta);
	
	virtual void DrawPrimitives();
	virtual void Draw();
	
	// parenting... needs a getChild function still.
	void setParent(Object *obj);
	void addChild(Object *obj);
	
	// animation
	void addState(int tweentype, double length);
	
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setColor(float r, float g, float b, float a);
	
	Matrix getMatrix();
	vec3 getPosition();
	vec3 getRotation();
	vec3 getScale();
	
	std::string name;
	
protected:
	void QueueUpdate();
	void Register();
	
	bool m_bNeedsUpdate;
	
	// animation
	unsigned m_frame;
	std::vector<AnimationState> m_states;
	Matrix m_matrix;
	Timer m_timer;
	
	// children
	std::vector<Object*> m_children;
	Object *m_parent;
		
	// hold these just for derived classes
	Texture m_texture;
	Shader m_shader;
	
	rgba m_color;
	vec3 m_position, m_rotation, m_scale;
};

void Object_Binding();

#endif
