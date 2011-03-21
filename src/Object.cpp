#include <GL/glew.h>
#include "Object.h"
#include "GameManager.h"
#include "LuaManager.h"
#include <SLB/SLB.hpp>
#include <glm/gtc/type_ptr.hpp>

Object::Object() : m_bNeedsUpdate(true), m_loop(false), m_frame(0), m_parent(NULL), m_align(glm::ivec2(0,0)), m_color(rgba(1.0)),
	m_position(vec3(0.0)), m_rotation(vec3(0.0)), m_scale(vec3(1.0)), m_size(vec3(1.0)), m_texture()
{
	m_shader.setProjectionMatrix(Game->ProjectionMatrix);
	m_texture.width = m_texture.height = 1;	
	
	addState(0,0);
	
	this->Register();
}

void Object::Register()
{
	Game->AddObject(this);
}

void Object::QueueUpdate()
{
	m_bNeedsUpdate = true;	
}

void Object::HandleMessage(std::string msg)
{
	// TODO
}

void Object::UpdateTweens(double delta)
{
	double time = m_timer.Ago();
	
	// need to update when a) it's queued and b) there's more animation to do.
	// XXX: extra updates when parented... most actors will be children of another.
	if (m_bNeedsUpdate || m_frame < m_states.size() || m_parent)
	{
		// update animation states.
		if (m_frame < m_states.size())
		{
			if (m_frame > 0) {
				AnimationState prev = m_states[m_frame-1];
				AnimationState next = m_states[m_frame];
				double duration = next.duration;
				
				m_position = interpolate(next.type, prev.position, next.position, duration, time);
				m_rotation = interpolate(next.type, prev.rotation, next.rotation, duration, time);
				m_scale = interpolate(next.type, prev.scale, next.scale, duration, time);
				m_size = interpolate(next.type, prev.size, next.size, duration, time);
				m_color = interpolate(next.type, prev.color, next.color, duration, time);
				
				QueueUpdate();
			}
			else
			{
				// update is already queued in this case, since it's usually only the first update.
				m_position = m_states[m_frame].position;
				m_rotation = m_states[m_frame].rotation;
				m_scale = m_states[m_frame].scale;
				m_size = m_states[m_frame].size;
				m_color = m_states[m_frame].color;
			}
			
			// this frame is done, next!
			if (time > m_states[m_frame].duration)
			{
				m_timer.Touch();
				m_frame++;
				if (m_loop && m_frame == m_states.size())
					m_frame = 0;
			}
		}
		
		m_matrix.Identity();
		
		// Copy position to adjust for alignment.
		vec3 align = m_position;

		// handle alignment
		if (m_align.x || m_align.y)
		{			
			// X align
			if (m_align.x == 1)
				align.x += m_size.x*0.5;
			else if (m_align.x == 2)
				align.x += -m_size.x*0.5;
			
			// Y align
			if (m_align.y == 1)
				align.y += m_size.y*0.5;
			else if (m_align.y == 2)
				align.y += -m_size.y*0.5;			
		}
		m_matrix.Translate(align);
		
		m_matrix.Rotate(m_rotation);		
		m_matrix.Scale(m_scale);
		m_matrix.Scale(m_size);
		
		if (m_parent)
			m_matrix.matrix = m_parent->getMatrix().matrix * m_matrix.matrix;
		
		m_bNeedsUpdate = false;
		
		Game->QueueRendering();
	}
}

void Object::Update(double delta)
{
	this->UpdateTweens(delta);
}

// These are up to derived classes.
void Object::DrawPrimitives() { }
void Object::Draw()
{
	m_shader.setModelViewMatrix(&m_matrix);
	
	// Bind shader and set uniforms
	m_shader.Bind();
	glUniform4fv(m_shader.getUniform("Color"), 1, glm::value_ptr(m_color));
	
	m_texture.Bind();
	
	this->DrawPrimitives();
}

// parenting... needs a getChild function still.
void Object::setParent(Object *obj)
{
	m_parent = obj;
}

void Object::addChild(Object *obj)
{
	m_children.push_back(obj);
}

/*
void Object::getChildByName(const char *name)
{
	
}
*/

// animation
void Object::addState(int tweentype, double length)
{
	// Push a copy of the current state onto the stack.
	AnimationState state;
	
	if (!m_states.empty())
		state = m_states.back();
	
	state.type = (TweenType)tweentype;
	state.duration = length;
	
	m_states.push_back(state);	
}

void Object::setAlign(int x, int y)
{
	m_align = glm::ivec2(x, y);
}

void Object::setPosition(float x, float y, float z)
{
	m_states.back().position = vec3(x, y, z);
	QueueUpdate();
}

void Object::setRotation(float x, float y, float z)
{
	m_states.back().rotation = vec3(x, y, z);
	QueueUpdate();
}

void Object::setSize(float x, float y, float z)
{
	m_states.back().size = vec3(x, y, z);
	QueueUpdate();
}

void Object::setScale(float x, float y, float z)
{
	m_states.back().scale = vec3(x, y, z);
	QueueUpdate();
}

void Object::setColor(float r, float g, float b, float a)
{
	m_states.back().color = rgba(r, g, b, a);
	QueueUpdate();
}

void Object::setLoop(bool enabled)
{
	m_loop = true;
}

Matrix Object::getMatrix()
{
	return m_matrix;
}

// Get object texture size.
float Object::getWidth()
{
	return m_size.x;
}

float Object::getHeight()
{
	return m_size.y;
}

vec3 Object::getPosition()
{
	return m_position;
}

vec3 Object::getRotation()
{
	return m_rotation;
}

vec3 Object::getSize()
{
	return m_size;
}

vec3 Object::getScale()
{
	return m_scale;
}

// Lua
#include <SLB/SLB.hpp>
void Object_Binding()
{
	SLB::Class<Object>("Object")
	.constructor()
	.set("Register", &Object::Register)
	.set("addState", &Object::addState)
	.set("addChild", &Object::addChild)
	.set("setAlign", &Object::setAlign)
	.set("setLoop", &Object::setLoop)
	.set("setParent", &Object::setParent)
	.set("setPosition", &Object::setPosition)
	.set("setRotation", &Object::setRotation)
	.set("setScale", &Object::setScale)
	.set("setColor", &Object::setColor);
}
