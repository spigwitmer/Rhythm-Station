#include "ScreenTestDrawing.h"
#include <GL3/gl3w.h>
#include <glsw.h>
#include "utils/Logger.h"

#define GLM_FORCE_SSE2 1
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer/gl3/Shader.h"
#include "renderer/common/Error.h"

using namespace std;

REGISTER_SCREEN(ScreenTestDrawing);

ShaderProgram *prog;

ScreenTestDrawing::ScreenTestDrawing(string name) : Screen(name)
{
}

void ScreenTestDrawing::Init()
{
	// It's safe to make one of these provided we aren't making extra windows.
	ShaderStage vs, fs;
	
	prog = new ShaderProgram();
	
	if (const char* err = glswGetError())
		LOG->Info("%s", err);
	
	// Generate a Vertex Array Object, stores 
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, &buf[0]);
	
	vs.Load(SHADER_VERTEX, "Generic.GL32.Vertex");
	fs.Load(SHADER_FRAGMENT, "Generic.GL32.Fragment");

	prog->Attach(vs);
	prog->Attach(fs);
	
	glBindAttribLocation(prog->GetObject(), 0, "Position");
	CheckError();
	
	LOG->Info("Linking program");	
	prog->Link();
	prog->Bind();
	
	CheckError();
	
	glBindVertexArray(vao);
	
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
	glDeleteBuffers(2, buf);	
	
	delete prog;
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
	glUniformMatrix4fv(glGetUniformLocation(prog->GetObject(), "ModelViewProjection"), 1, false, glm::value_ptr(matrix));

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);	
}
