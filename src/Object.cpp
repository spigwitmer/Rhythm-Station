#include <GL/glew.h>
#include "Object.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "FileManager.h"
#include "Screen.h"
#include "Logger.h"
#include "PNGFile.h"

Object::Object() : m_bNeedsUpdate(true), m_bDepthClear(false), m_color(rgba(1.0)), m_texture(),
	m_pos(vec3(0.0)), m_rot(vec3(0.0)), m_scale(vec3(1.0))
{
	m_shader.SetProjectionMatrix(Game->ProjectionMatrix); // XXX
	m_shader.Bind();
	m_color_uniform = glGetUniformLocation(m_shader.ptr, "Color");
	m_texture.width = m_texture.height = 1;
	m_parent = NULL;
	
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
	
	// Register in scene.
	Register();
}

Object::~Object()
{
	mesh.Free();
	
	if (m_texture.ptr != 0)
		ResourceManager::FreeResource(m_texture);
}

void Object::SetParent(Object *obj)
{
	m_parent = obj;
}

void Object::AddChild(Object *obj)
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

void Object::AddState()
{
	m_states["Init"].push_back(m_current);
}

void Object::Color(rgba col)
{
	m_color = col;
	QueueUpdate();
}


// TODO: add to tweens
void Object::Translate(vec3 pos)
{
	m_pos = pos;
	QueueUpdate();
}

void Object::Rotate(vec3 rot)
{
	m_rot = rot;
	QueueUpdate();
}

void Object::Scale(vec3 scale)
{
	m_scale = scale;
	QueueUpdate();
}

// update tweens and stuff
void Object::Update(double delta)
{
	if (m_bNeedsUpdate)
	{
		m_matrix.Identity();
		
		// if we have a parent, copy the translation/rotation/scale.
		if (m_parent)
			m_matrix.Translate(m_parent->GetTranslation());
		m_matrix.Translate(m_pos);
		
		if (m_parent)
			m_matrix.Scale(m_parent->GetScale());
		m_matrix.Scale(m_scale);
		m_matrix.Scale(m_texture.width, m_texture.height, 1.0);
		
		if (m_parent)
			m_matrix.Rotate(m_parent->GetRotation());
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
	.set("Translate", &Object::Translate3f)
	.set("Rotate", &Object::Rotate3f)
	.set("Scale", &Object::Scale3f)
	.set("Parent", &Object::SetParent)
	.set("Color", &Object::Color4f);
}
