#include "ScreenTestDrawing.h"
#include <GL3/gl3w.h>
#include <glsw.h>
#include "managers/DisplayManager.h"
#include "utils/Logger.h"

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
	
	// Identity.
	float matrix[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	glUniformMatrix4fv(glGetUniformLocation(id, "ModelViewProjection"), 1, false, matrix);
	
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);	
	glEnableVertexAttribArray(0);
	
	// generate a 1D texture
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_1D, tex);
	unsigned data[2048];
	for (int i = 0; i<2048; i++)
		data[i] = 0x001000 * i;
	
	// Upload the pixel data
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, sizeof(data), 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	
	// IMPORTANT: Min/Mag filters are essential if you actually want to see things
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

void ScreenTestDrawing::Draw()
{	
	// Bind shader and VBOs
	glBindVertexArray(vao);
	
	glBindTexture(GL_TEXTURE_1D, tex);
	
	glUniform1i(glGetUniformLocation(id, "ColorTable"), 0);
	// Identity.
	float matrix[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	glUniformMatrix4fv(glGetUniformLocation(id, "ModelViewProjection"), 1, false, matrix);

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);	
}
