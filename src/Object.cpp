#include "globals.h"
#include "Object.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "Screen.h"
#include "Logger.h"

GLuint quad_vbo, quad_ibo;

void GenerateQuadBuffers() {
	// generic quad
	GLfloat verts[] = {
		-1, -1, 0, 0, 0, 0, 0, 0,
		 1, -1, 0, 1, 0, 0, 0, 0,
		-1,  1, 0, 0, 1, 0, 0, 0,
		 1,  1, 0, 1, 1, 0, 0, 0,
	};
	GLubyte indices[] = { 0, 8, 16, 24 }; // may be incorrect

	char stride = sizeof(float) * (3 + 2 + 3);

	glGenBuffers(1, &quad_vbo);
	glGenBuffers(1, &quad_ibo);
	
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// XXX: 0 offset = these don't really work.
	// coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0);

	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
}

void DeleteQuadBuffers() {
	glDeleteBuffers(1, &quad_vbo);
	glDeleteBuffers(1, &quad_ibo);
}

Object::Object() : m_texture(0), m_color(rgba(1.0)) {
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
	_path = _path.substr(_path.size()-3, _path.size());
	if (!strcmp(_path.c_str(), "png"))
		Log->Print("PNG file detected.");
	else
		Log->Print("Unknown file type.");

}

void Object::HandleMessage(std::string _msg) {

}

void Object::Color(rgba col) {
	m_color = col;
}

// TODO: add to tweens
void Object::Translate(vec3 pos) {
	m_mat.Translate(pos);
}

void Object::Rotate(vec3 rot) {
	m_mat.Rotate(rot);
}

void Object::Scale(vec3 scale) {
	m_mat.Scale(scale);
}

// update tweens and stuff
void Object::Update(double delta) {
	m_shader.SetModelViewMatrix(&m_mat);

	// TODO: only update when needed.
	Game->QueueRendering();

	m_mat.Rotate(20 * delta, 1, 0.75, 0);

	return;
}

void Object::Draw() {
	m_shader.Bind();
	m_shader.SetUniforms();

	glUniform4f(m_color_uniform, m_color.r, m_color.g, m_color.b, m_color.a);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_ibo);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
