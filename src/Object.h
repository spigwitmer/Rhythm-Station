#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <vector>
#include <map>

#include "Matrix.h"
#include "Message.h"
#include "Timer.h"
#include "Tween.h"
#include "Type.h"

class AnimationState
{
	// Make sure things don't go uninitialized!
	AnimationState() : type(TWEEN_SLEEP), duration(0.0), color(rgba(1.0)),
	position(vec3(0.0)), rotation(vec3(0.0)), scale(vec3(1.0)) { }
	
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
	
	virtual void Load(std::string path) = 0;
	
	void HandleMessage(std::string msg);
	void Update(double delta);
	
	void DrawPrimitives();
	virtual void Draw();
	
	// parenting... needs a getChild function still.
	void setParent(Object *obj);
	void addChild(Object *obj);
	
	// animation
	void addState(int tweentype, double length);
	
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);
			
	Matrix getMatrix();
	vec3 getPosition();
	vec3 getRotation();
	vec3 getScale();
	
	std::string name;
	
protected:
	
	void Register();
	
	bool m_bNeedsUpdate;
	unsigned m_frame;
	std::vector<AnimationState> m_states;
	
	std::vector<Object*> m_children;
	Object *m_parent;
	
	Matrix m_matrix;
	Timer m_timer;

	vec3 m_position, m_rotation, m_scale;
};

void Object_Binding();

#endif
