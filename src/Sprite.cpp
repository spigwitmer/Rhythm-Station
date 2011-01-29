#include <GL/glew.h>
#include "Object.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "FileManager.h"
#include "Screen.h"
#include "Logger.h"
#include "PNGFile.h"

Object::Object() : m_bNeedsUpdate(true), m_bDepthClear(false), m_frame(0),
	m_color(rgba(1.0)),m_texture(), m_pos(vec3(0.0)), m_rot(vec3(0.0)), m_scale(vec3(1.0))
{
	m_shader.SetProjectionMatrix(Game->ProjectionMatrix); // XXX
	m_shader.Bind();
	m_color_uniform = glGetUniformLocation(m_shader.id, "Color");
	m_texture.width = m_texture.height = 1;
	m_parent = NULL;
	
	// Register in scene.
	Register();
	addState(0,0);
}

Object::~Object()
{
	mesh.Free();
	
	if (m_texture.ptr != 0)
		ResourceManager::FreeResource(m_texture);
}

void Object::addChild(Object *obj)
{
	m_children.push_back(obj);
}

void Object::Register()
{
	Screen *scr = Game->GetTopScreen();
	scr->AddObject(this);
}

void Object::QueueUpdate()
{
	m_bNeedsUpdate = true;
}

void Object::Load(std::string _path)
{
	std::string path = FileManager::GetFile(_path);
	std::string ext = path.substr(path.size()-3, path.size());
	Log->Print("Loading \"%s\" (type = %s)", _path.c_str(), ext.c_str());
	
	if (ext.compare("png") == 0)
	{
		// XXX: All sprites could be using the same 1u square here.
		// Create a VBO (1u square)
		MeshData verts[4];
		float vertices[] = {
			-0.5, -0.5, 0, 0, 0, 0, 0, 0,
			-0.5,  0.5, 0, 0, 0, 0, 0, 1,
			0.5, -0.5, 0, 0, 0, 0, 1, 0,
			0.5,  0.5, 0, 0, 0, 0, 1, 1,
		};
		unsigned indices[] = {
			0, 1, 2,
			1, 2, 3
		};
		memcpy(&verts[0].Position.x, vertices, sizeof(vertices));
		mesh.Load(verts, indices, 4, 6);

		Texture tex;
		
		if (!ResourceManager::GetResource(path,tex))
		{
			PNGFile png;
			tex = png.Load(path);
			
			// register this so we don't load it again.
			ResourceManager::Add(tex);
		}
		
		if (tex.ptr != 0)
			m_texture = tex;
	}
	else
		Log->Print("Unknown file type: %s.", ext.c_str());
		
	QueueUpdate();
}

void Object::HandleMessage(std::string _msg)
{
	// TODO
}

void Object::addState(int tweentype, double length)
{
	// Push a copy of the current state onto the stack.
	AnimationState state;
	if (m_states.size() > 0)
		state = m_states.back();
	state.type = (TweenType)tweentype;
	state.duration = length;
	
	m_states.push_back(state);
}

void Object::setParent(Object *obj)
{
	m_parent = obj;
}

void Object::setColor(float r, float g, float b, float a)
{
	m_states.back().color = rgba(r, g, b, a);
	QueueUpdate();
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

void Object::setScale(float x, float y, float z)
{
	m_states.back().scale = vec3(x, y, z);
	QueueUpdate();
}

// update tweens and stuff
void Object::Update(double delta)
{
	double time = m_timer.Ago();
	
	// need to update when a) it's queued and b) there's more animation to do.
	if (m_bNeedsUpdate || m_frame < m_states.size())
	{
		// update animation states.
		if (m_frame < m_states.size())
		{
			if (m_frame > 0) {
				AnimationState prev = m_states[m_frame-1];
				AnimationState next = m_states[m_frame];
				double duration = next.duration;
				
				m_pos = interpolate(next.type, prev.position, next.position, duration, time);
				m_rot = interpolate(next.type, prev.rotation, next.rotation, duration, time);
				m_scale = interpolate(next.type, prev.scale, next.scale, duration, time);
				
				QueueUpdate();
			}
			else
			{
				// update is already queued in this case, since it's usually only the first update.
				m_pos = m_states[m_frame].position;
				m_rot = m_states[m_frame].rotation;
				m_scale = m_states[m_frame].scale;
			}
			
			// this frame is done, next!
			if (time > m_states[m_frame].duration)
			{
				m_timer.Touch();
				m_frame++;
			}
		}
		
		m_matrix.Identity();
		
		// if we have a parent, copy the translation/rotation/scale.
		if (m_parent)
			m_matrix.Translate(m_parent->getTranslation());
		m_matrix.Translate(m_pos);
		
		if (m_parent)
			m_matrix.Scale(m_parent->getScale());
		m_matrix.Scale(m_scale);
		m_matrix.Scale(m_texture.width, m_texture.height, 1.0);
		
		if (m_parent)
			m_matrix.Rotate(m_parent->getRotation());
		m_matrix.Rotate(m_rot);
		
		m_bNeedsUpdate = false;
		Game->QueueRendering();
	}
	
	m_shader.SetModelViewMatrix(&m_matrix);
}

void Object::DepthClear(bool enabled)
{
	m_bDepthClear = enabled;
}

void Object::Draw()
{
	// Use so 3D objects don't collide.
	if (m_bDepthClear)
		glClear(GL_DEPTH_BUFFER_BIT);
	
	glDisable(GL_DEPTH_TEST); // XXX
	
	// Bind shader and set uniforms
	m_shader.Bind();
	m_texture.Bind();
	glUniform4f(m_color_uniform, m_color.r, m_color.g, m_color.b, m_color.a);
	
	// Draw!
	mesh.Draw();
}

// Lua
#include <SLB/SLB.hpp>
void Object_Binding()
{
	SLB::Class<Object>("Object")
	.constructor()
	.set("Load", &Object::Load)
	.set("addState", &Object::addState)
	.set("addChild", &Object::addChild)
	.set("setParent", &Object::setParent)
	.set("setPosition", &Object::setPosition)
	.set("setRotate", &Object::setRotation)
	.set("setScale", &Object::setScale)
	.set("setColor", &Object::setColor);
}
