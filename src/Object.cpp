#include <GL/glew.h>
#include "Object.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "FileManager.h"
#include "Screen.h"
#include "Logger.h"
#include "PNGLoader.h"

Object::Object() : m_bNeedsUpdate(true), m_bDepthClear(false), m_color(rgba(1.0)), m_texture(),
	m_pos(vec3(0.0)), m_rot(vec3(0.0)), m_scale(vec3(0.0))
{
	m_shader.SetProjectionMatrix(Game->ProjectionMatrix);
	m_shader.Bind();
	m_color_uniform = glGetUniformLocation(m_shader.ptr, "Color");
	CreateBuffers();
	m_texture.width = m_texture.height = 1;

	m_parent = NULL;

	Register();
}

Object::~Object()
{
	DeleteBuffers();
	if (m_texture.ptr != 0)
		ResourceManager::FreeResource(m_texture);
}

void Object::SetParent(Object* obj)
{
	m_parent = obj;
}

void Object::AddChild(Object* obj)
{
	m_children.push_back(obj);
}

// TODO: Move to RenderManager.
// TODO TODO: just make a util namespace for this stuff.
void Object::CreateBuffers()
{
	// quad (2 tris)
	GLfloat verts[] = {
		// pos[3] nor[3] tex[2]
		-1, -1, 0, 0, 0, 0, 0, 0,
		-1,  1, 0, 0, 0, 0, 0, 1,
		 1, -1, 0, 0, 0, 0, 1, 0,
		 1,  1, 0, 0, 0, 0, 1, 1,
	};
	GLubyte indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	// far more useful on complex objects.
	GLubyte stride = sizeof(GLfloat) * 8;
	m_vertices = sizeof(indices) / sizeof(GLubyte);

	glGenBuffers(2, m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) (sizeof(GLfloat) * (0)));
	glVertexAttribPointer(NORMAL_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) (sizeof(GLfloat) * (3)));
	glVertexAttribPointer(COORD_ARRAY, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) (sizeof(GLfloat) * (6)));
}

void Object::DeleteBuffers()
{
	glDeleteBuffers(2, m_vbo);
}

void Object::Register()
{
	Screen* scr = Game->GetTopScreen();
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
			PNGLoader png;
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
	if (m_parent)
		m_matrix = m_parent->GetMatrix();

	if (m_bNeedsUpdate)
	{
		m_matrix.Identity();
		m_matrix.Translate(m_pos);
		m_matrix.Scale(m_scale);
		m_matrix.Scale(m_texture.width*0.5f, m_texture.height*0.5f, 1.0);
		m_matrix.Rotate(m_rot);
		m_bNeedsUpdate = false;

		Game->QueueRendering();
	}

	m_shader.SetModelViewMatrix(&m_matrix);
}

void Object::AssignBuffer(GLuint *buf, int verts)
{
	m_vbo[0] = buf[0];
	m_vbo[1] = buf[1];
	m_vertices = verts;

	QueueUpdate();
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

	GLubyte stride = sizeof(GLfloat) * 8;
	// need a buffer type, probably.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);

	glVertexAttribPointer(VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) (sizeof(GLfloat) * (0)));
	glVertexAttribPointer(NORMAL_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) (sizeof(GLfloat) * (3)));
	glVertexAttribPointer(COORD_ARRAY, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) (sizeof(GLfloat) * (6)));

	m_shader.Bind();
	m_texture.Bind();

	glUniform4f(m_color_uniform, m_color.r, m_color.g, m_color.b, m_color.a);
	glDrawElements(GL_TRIANGLES, m_vertices, GL_UNSIGNED_BYTE, NULL);
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
	.set("Color", &Object::Color4f);
}
