#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <vector>
#include <map>

#include "renderer/Matrix.h"
#include "Message.h"
#include "utils/Timer.h"
#include "Tween.h"
#include "Type.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

struct AnimationState
{
	// Make sure things don't go uninitialized!
	AnimationState() : type(TWEEN_SLEEP), duration(0.0), color(rgba(1.0)),
	position(vec3(0.0)), rotation(vec3(0.0)), scale(vec3(1.0)), size(vec3(1.0)) { }
	
	AnimationState operator = (const AnimationState &anim)
	{
		type = anim.type;
		duration = anim.duration;
		position = anim.position;
		rotation = anim.rotation;
		scale = anim.scale;
		color = anim.color;
		size = anim.size;
		return *this;
	}
	
	TweenType type;
	double duration;
	
	rgba color;
	vec3 position, rotation, scale, size;
};

class Object : public MessageListener
{
public:
	Object();
	
	virtual void Load(std::string path) { }
	
	void HandleMessage(std::string msg);
	void UpdateTweens(double delta);
	
	virtual void Update(double delta);
	
	virtual void DrawPrimitives();
	virtual void Draw();
	
	void Register();
	
	// parenting... needs a getChild function still.
	void SetParent(Object *obj);
	void AddChild(Object *obj);
	
	// animation
	void AddState(int tweentype, double length);
	void SetAlign(int x, int y);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetLoop(bool enabled);
	void SetScale(float x, float y, float z);
	void SetSize(float x, float y, float z);
	void SetColor(float r, float g, float b, float a);
	
	Matrix GetMatrix()	const { return m_matrix; }
	
	float GetWidth()	const { return m_size.x; }
	float GetHeight()	const { return m_size.y; }
	
	vec3 GetPosition()	const { return m_position; }
	vec3 GetRotation()	const { return m_rotation; }
	vec3 GetScale() 	const { return m_scale; }
	vec3 GetSize()		const { return m_size; }
	
	std::string name;
	
protected:
	void QueueUpdate();
	
	bool m_bNeedsUpdate, m_loop;
	
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
	
	glm::ivec2 m_align;
	rgba m_color;
	vec3 m_position, m_rotation, m_scale, m_size;
};

void Object_Binding();

#endif
