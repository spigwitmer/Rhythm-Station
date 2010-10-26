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
		// pos	tex	 normals
		-1, -1, 0,	0, 0,	 0, 0, 0,
		 1, -1, 0,	1, 0,	 0, 0, 0,
		-1,  1, 0,	0, 1,	 0, 0, 0,
		 1,  1, 0,	1, 1,	 0, 0, 0,
	};
	GLubyte indices[] = { 0, 1, 2, 3 };

	char stride = sizeof(GLfloat) * (3 + 2 + 3);

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

	// normals
//	glEnableVertexAttribArray(NORMAL_ARRAY);
//	glVertexAttribPointer(COORD_ARRAY, 3, GL_FLOAT, GL_FALSE, stride, OFFSET(5));
}
#undef OFFSET

void DeleteQuadBuffers() {
	glDeleteBuffers(1, &quad_vbo);
	glDeleteBuffers(1, &quad_ibo);
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
	const char* ext = _path.substr(_path.size()-3, _path.size()).c_str();
	if (!strcmp(ext, "png")) {
		Log->Print("Loading PNG file...");
		PNGLoader png;
		Texture tex = png.Load(_path);
		if (tex.ptr != 0) {
			Log->Print("Loaded successfully.");
			this->m_texture = tex;
		}
		m_mat.Scale(vec3(tex.width/2,tex.height/2,1.0));
	}
	else if (!strcmp(ext, "obj")) {
		Log->Print("Loading OBJ file...");
		Log->Print("Failed; Not yet implemented.");
	}
	else
		Log->Print("Unknown file type.");

}

void Object::HandleMessage(std::string _msg) {

}

void Object::AddState() {
	if (m_states.find("Init") != m_states.end()) {
		Log->Print("Found Init.");
		m_states.find("Init")->second.push_back(m_mat);
	}
	else {
		Log->Print("Init not found. Creating.");
		std::pair<std::string, std::vector<Matrix> > tmp;
		tmp.first = "Init";
		tmp.second.push_back(m_mat);
		m_states.insert(tmp);
	}
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

	if (m_states.find("Init") != m_states.end())
		Log->Print("test");

	// TODO: only update when needed.
	Game->QueueRendering();

	return;
}

void Object::Draw() {
	glActiveTexture(GL_TEXTURE0);
	m_texture.Bind();

	if (!m_vbo) {
		glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_ibo);
	}

	m_shader.Bind();
	m_shader.SetUniforms();

	glUniform4f(m_color_uniform, m_color.r, m_color.g, m_color.b, m_color.a);

	if (!m_vbo)
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, NULL);
}
