#include <GL/glew.h>
#include "Object.h"
#include "GameManager.h"
#include "FileManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Screen.h"
#include "Logger.h"
#include "PNGLoader.h"

Object::Object() : m_bNeedsUpdate(true), m_color(rgba(1.0)), m_texture(),
	m_pos(vec3(0.0)), m_rot(vec3(0.0)), m_scale(vec3(0.0))
{
	m_shader.SetProjectionMatrix(Game->ProjectionMatrix);
	m_shader.Bind();
	m_color_uniform = glGetUniformLocation(m_shader.ptr, "Color");
	CreateBuffers();

	m_parent = NULL;
}

Object::~Object()
{
	DeleteBuffers();
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
#define OFFSET(P) (const GLvoid*) (sizeof(GLfloat) * (P))
void Object::CreateBuffers()
{
	// triangle strip quad
	GLfloat verts[] = {
		// pos[3] nor[3] tex[2]
		-1, -1, 0, 0, 0, 0, 0, 0,
		 1, -1, 0, 0, 0, 0, 1, 0,
		-1,  1, 0, 0, 0, 0, 0, 1,
		 1,  1, 0, 0, 0, 0, 1, 1,
	};
	// far more useful on complex objects.
	int components = 8;
	GLubyte stride = sizeof(GLfloat) * components;
	m_vertices = sizeof(verts) / sizeof(GLfloat) / components;

	glGenBuffers(1, &m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	// vertices
	glEnableVertexAttribArray(VERTEX_ARRAY);
	glVertexAttribPointer(VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, OFFSET(0));

	// normals
	glEnableVertexAttribArray(NORMAL_ARRAY);
	glVertexAttribPointer(NORMAL_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, OFFSET(3));

	// coords
	glEnableVertexAttribArray(COORD_ARRAY);
	glVertexAttribPointer(COORD_ARRAY, 2, GL_FLOAT, GL_FALSE, stride, OFFSET(6));
}
#undef OFFSET

void Object::DeleteBuffers()
{
	glDeleteBuffers(1, &m_vbo);
}

// should this be automatic?
void Object::Register()
{
	Screen* scr = Scene->GetTopScreen();
	scr->AddObject(this);
}

void Object::QueueUpdate()
{
	m_bNeedsUpdate = true;
}

void Object::Load(std::string _path)
{
	std::string path = File->GetFile(_path);
	const char* ext = path.substr(path.size()-3, path.size()).c_str();
	Log->Print("Loading \"%s\" (type = %s)", _path.c_str(), ext);
	if (!strcmp(ext, "png"))
	{
		PNGLoader png;
		Texture tex = png.Load(path);
		if (tex.ptr != 0)
			m_texture = tex;
	}
	else
		Log->Print("Unknown file type: %s.", ext);

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

void Object::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	m_shader.Bind();
	m_texture.Bind();

	m_shader.SetUniforms();

	glUniform4f(m_color_uniform, m_color.r, m_color.g, m_color.b, m_color.a);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertices);
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
	.set("Color", &Object::Color4f)
	.set("Register", &Object::Register);
}
