#include <GL/glew.h>
#include "globals.h"
#include "Object.h"
#include "GameManager.h"
#include "FileManager.h"
#include "SceneManager.h"
#include "Screen.h"
#include "Logger.h"
#include "PNGLoader.h"

static GLuint quad_vbo[2];

#define OFFSET(P) (const GLvoid*) (sizeof(GLfloat) * (P))
#define STRIDE (sizeof(GLfloat) * (3+2+3))
void GenerateQuadBuffers() {
	// triangle strip quad
	GLfloat verts[] = {
		// pos	tex	 normals
		-1, -1, 0,	0, 0,	 0, 0, 0,
		 1, -1, 0,	1, 0,	 0, 0, 0,
		-1,  1, 0,	0, 1,	 0, 0, 0,
		 1,  1, 0,	1, 1,	 0, 0, 0,
	};
	GLubyte indices[] = { 0, 1, 2, 3 };

	glGenBuffers(2, quad_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// vertices
	glEnableVertexAttribArray(VERTEX_ARRAY);
	glVertexAttribPointer(VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, STRIDE, OFFSET(0));

	// coords
	glEnableVertexAttribArray(COORD_ARRAY);
	glVertexAttribPointer(COORD_ARRAY, 2, GL_FLOAT, GL_FALSE, STRIDE, OFFSET(3));

	// normals
	glEnableVertexAttribArray(NORMAL_ARRAY);
	glVertexAttribPointer(NORMAL_ARRAY, 3, GL_FLOAT, GL_FALSE, STRIDE, OFFSET(5));
}

void DeleteQuadBuffers() {
	glDeleteBuffers(2, quad_vbo);
}

Object::Object() : m_vbo(0), m_color(rgba(1.0)), m_texture() {
	m_shader.SetProjectionMatrix(g_projection_matrix);
	m_shader.Bind();
	m_color_uniform = glGetUniformLocation(m_shader.ptr, "Color");
}

Object::~Object() {
	// nothing to clean up yet
}

void Object::Register() {
	Screen* scr = Scene->GetTopScreen();
	scr->AddObject(this);
}

void Object::Load(std::string _path) {
	_path = File->GetFile(_path);
	const char* ext = _path.substr(_path.size()-3, _path.size()).c_str();
	if (!strcmp(ext, "png")) {
		Log->Print("Loading PNG file...");
		PNGLoader png;
		Texture tex = png.Load(_path);
		if (tex.ptr != 0) {
			Log->Print("Loaded successfully.");
			this->m_texture = tex;
		}
//		m_matrix.Translate(vec3((tex.width % 2) ? 0 : 0.5,(tex.height % 2) ? 0 : 0.5,0));
		m_matrix.Scale(vec3(tex.width/2,tex.height/2,1.0));
	}
	else if (!strcmp(ext, "obj")) {
		Log->Print("Loading OBJ file...");
		Log->Print("Failed; Not yet implemented.");
	}
	else
		Log->Print("Unknown file type.");

}

void Object::HandleMessage(std::string _msg) {
	// TODO
}

void Object::AddState() {
	m_states["Init"].push_back(m_current);
}

void Object::Color(rgba col) {
	m_color = col;
}


// TODO: add to tweens
void Object::Translate(vec3 pos) {
	m_matrix.Translate(pos);
}

void Object::Rotate(vec3 rot) {
	m_matrix.Rotate(rot);
}

void Object::Scale(vec3 scale) {
	m_matrix.Scale(scale);
}

// update tweens and stuff
void Object::Update(double delta) {
	m_shader.SetModelViewMatrix(&m_matrix);

	// TODO: only update when needed.
	return;

	Game->QueueRendering();
}

void Object::Perspective(float fov) {
	// leaks!
	Matrix* mat = new Matrix();
	mat->Perspective(60, 854.0/480.0, 1, 500);
	m_shader.SetProjectionMatrix(mat);
}

void Object::Draw() {
	m_texture.Bind();

	// TODO: only bind buffers if the same set isn't already bound.
	if (!m_vbo) {
		glBindBuffer(GL_ARRAY_BUFFER, quad_vbo[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_vbo[1]);
	}

	m_shader.Bind();
	m_shader.SetUniforms();

	glUniform4f(m_color_uniform, m_color.r, m_color.g, m_color.b, m_color.a);

	if (!m_vbo)
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, NULL);
}
#undef OFFSET
#undef STRIDE

// Lua
#include <SLB/SLB.hpp>
void Object_Binding() {
	SLB::Class<Object>("Object")
	.constructor()
	.set("Load", &Object::Load)
	.set("Translate", &Object::Translate3f)
	.set("Rotate", &Object::Rotate3f)
	.set("Scale", &Object::Scale3f)
	.set("Color", &Object::Color4f)
	.set("Perspective", &Object::Perspective)
	.set("Register", &Object::Register);

	Log->Print("Registered Object class");
}
