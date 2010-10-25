#include <GL/glew.h>
#include "globals.h"
#include "Object.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "Screen.h"
#include "Logger.h"
#include "PNGLoader.h"

GLuint quad_vbo, quad_ibo;

#define OFFSET(P) (const GLvoid*) (sizeof(GLfloat) * (P))
void GenerateQuadBuffers() {
	// triangle strip quad
	GLfloat verts[] = {
		-1, -1, 0,	0, 0,
		 1, -1, 0,	1, 0,
		-1,  1, 0,	0, 1,
		 1,  1, 0,	1, 1,
	};
	GLubyte indices[] = { 0, 1, 2, 3 };

	char stride = sizeof(GLfloat) * (3 + 2);

	glGenBuffers(1, &quad_vbo);
	glGenBuffers(1, &quad_ibo);
	
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// vertices
	glEnableVertexAttribArray(VERTEX_ARRAY);
	glVertexAttribPointer(VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, OFFSET(0));

	// coords
	glEnableVertexAttribArray(COORD_ARRAY);
	glVertexAttribPointer(COORD_ARRAY, 2, GL_FLOAT, GL_FALSE, stride, OFFSET(3));
}
#undef OFFSET

void DeleteQuadBuffers() {
	glDeleteBuffers(1, &quad_vbo);
	glDeleteBuffers(1, &quad_ibo);
}

Object::Object() : m_texture(), m_color(rgba(1.0)) {
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
	const char* ext = _path.substr(_path.size()-3, _path.size()).c_str();
	if (!strcmp(ext, "png")) {
		Log->Print("Path ext is PNG, trying to load.");
		PNGLoader png;
		Texture tex = png.Load(_path);
		if (tex.ptr != 0) {
			Log->Print("PNG file Loaded successfully.");
			this->m_texture = tex;
		}
		m_mat.Scale(vec3(tex.width/2,tex.height/2,1.0));
	}
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

//	m_mat.Rotate(20 * delta, 1, 0.75, 0);

	return;
}

void Object::Draw() {
	glActiveTexture(GL_TEXTURE0);
	m_texture.Bind();

	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_ibo);

	m_shader.Bind();
	m_shader.SetUniforms();

	glUniform4f(m_color_uniform, m_color.r, m_color.g, m_color.b, m_color.a);

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, NULL);

//	glUniform4f(m_color_uniform, 1, 1, 1, 1);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, NULL);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
