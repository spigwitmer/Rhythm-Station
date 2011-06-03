#include <GL/glew.h>
#include "Context.h"
#include "utils/Logger.h"
#include <string>

using namespace std;

Context::Context() :
	m_MaxAttributes(0),
	m_MaxUniforms(0)
{
}

Context::~Context()
{
}

int GetGLValue(const char* text, int param)
{
	int attr = 0;
	glGetIntegerv(param, &attr);
	LOG->Info(text, attr);

	return attr;
}

bool ComplainAbout(string text)
{
	LOG->Warn("Insufficient number of " + text + " supported. ");

	return true;
}

void Context::Init()
{
	LOG->Info("Checking hardware capabilities...");
	m_MaxAttributes = GetGLValue("-> Max Vertex Attributes: %d.", GL_MAX_VERTEX_ATTRIBS);
	m_MaxUniforms = GetGLValue("-> Max Uniform Components: %d.", GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);

	bool err = false;

	if (m_MaxAttributes < 8)
		err = ComplainAbout("Vertex Attributes");
	if (m_MaxUniforms < 512)
		err = ComplainAbout("Fragment Uniforms");

	if (!err)
		LOG->Info("Everything appears to be acceptable.");
}

