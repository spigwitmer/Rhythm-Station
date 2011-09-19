#include "ScreenTestDrawing.h"
#include <GL3/gl3w.h>
#include <glsw.h>
#include "managers/DisplayManager.h"
#include "utils/Logger.h"

#include <glm/glm.hpp>
#include <glm/gtx/simd_vec4.hpp>
#include <glm/gtx/simd_mat4.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

REGISTER_SCREEN(ScreenTestDrawing);

ScreenTestDrawing::ScreenTestDrawing(string name) : Screen(name)
{
	// It's safe to make one of these provided we aren't making extra windows.
	DisplayManager display;
	
	vss = glswGetShader("Generic.GL32.Vertex");
	fss = glswGetShader("Generic.GL32.Fragment");
	
	const char* err = glswGetError();
	if (err != NULL)
		LOG->Info("%s", err);
	
	// Generate a Vertex Array Object, stores 
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, &buf[0]);
	
	id = glCreateProgram();
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vs, 1, &vss, NULL);
	glShaderSource(fs, 1, &fss, NULL);
	
	glCompileShader(vs);
	glCompileShader(fs);
	
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	
	glBindAttribLocation(id, 0, "Position");
	glLinkProgram(id);
	
	// make sure nothing went wrong
	if (!(log = display.GetInfoLog(vs)).empty()) LOG->Info("Vertex shader log: %s", log.c_str());	
	if (!(log = display.GetInfoLog(fs)).empty()) LOG->Info("Fragment shader log: %s", log.c_str());
	if (!(log = display.GetInfoLog(id)).empty()) LOG->Fatal("Shader program log: %s", log.c_str());
	
	glBindVertexArray(vao);
	
	glUseProgram(id);
	
	float verts[] = { -1.0, -1.0, 0.0, -1.0, 1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0 };
	unsigned indices[] = { 0, 1, 2, 3 };
		
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);	
	glEnableVertexAttribArray(0);
}

ScreenTestDrawing::~ScreenTestDrawing()
{
	// cleanup:
	glDeleteVertexArrays(1, &vao);
	
	glDetachShader(id, vs);
	glDetachShader(id, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	glDeleteProgram(id);
	glDeleteBuffers(2, buf);	
}

void ScreenTestDrawing::HandleMessage(const Message &msg)
{
	// todo
}

void ScreenTestDrawing::UpdateInternal(double delta)
{
	// todo
}

// no sense making this a bunch of times.
static glm::mat4 matrix(1.0);

void ScreenTestDrawing::Draw()
{
	// Bind shader and VBOs
	glBindVertexArray(vao);
	
	// Identity.
	glUniformMatrix4fv(glGetUniformLocation(id, "ModelViewProjection"), 1, false, glm::value_ptr(matrix));

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);	
}

