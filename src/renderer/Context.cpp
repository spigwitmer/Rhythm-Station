#include <GL/glew.h>
#include "Context.h"
#include "utils/Logger.h"
#include <string>

using namespace std;

Context::Context() :
	m_MaxAnisotropy(0),
	m_MaxAttributes(0),
	m_MaxUniforms(0)
{
}

Context::~Context()
{
}

static int GetGLValue(const char* text, int param)
{
	int attr = 0;
	glGetIntegerv(param, &attr);
	LOG->Info(text, attr);

	return attr;
}

static bool ComplainAbout(string text)
{
	LOG->Warn("Insufficient number of " + text + " supported. ");

	return true;
}

void Context::Init()
{
	bool err = false;

	LOG->Info("Checking hardware capabilities...");

	if (!glewIsSupported("GL_ARB_fragment_program"))
		LOG->Warn("Fragment programs don't appear to be supported!");

	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
		m_MaxAnisotropy = GetGLValue("-> Max Anisotropic Filtering: %dx.", GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT);

	m_MaxAttributes = GetGLValue("-> Max Vertex Attributes: %d.", GL_MAX_VERTEX_ATTRIBS);
	m_MaxUniforms = GetGLValue("-> Max Uniform Components: %d.", GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);

	if (m_MaxAttributes < 8)
		err = ComplainAbout("Vertex Attributes");
	if (m_MaxUniforms < 512)
		err = ComplainAbout("Fragment Uniforms");

	if (!err)
		LOG->Info("Everything appears to be acceptable.");
}

