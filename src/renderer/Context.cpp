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

static void GetGLValue(int *target, const char *text, int param)
{
	glGetIntegerv(param, target);
	LOG->Info("-> %s: %d.", text, *target);
}

static bool ComplainAbout(string text, int check, int limit)
{
	if (check < limit)
	{
		LOG->Warn("Insufficient number of " + text + " supported. ");
		return true;
	}

	return false;
}

void Context::Init()
{
	bool err = false;

	LOG->Info("Checking hardware capabilities...");

	if (!glewIsSupported("GL_ARB_fragment_program"))
		LOG->Warn("Fragment programs don't appear to be supported!");

	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
		GetGLValue(&m_MaxAnisotropy, "Max Anisotropic Filtering", GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT);

	GetGLValue(&m_MaxAttributes, "Max Vertex Attributes", GL_MAX_VERTEX_ATTRIBS);
	GetGLValue(&m_MaxUniforms, "Max Uniform Components", GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);

	err = ComplainAbout("Vertex Attributes", m_MaxAttributes, 8);
	err = ComplainAbout("Fragment Uniforms", m_MaxUniforms, 512);

	if (!err)
		LOG->Info("Everything appears to be acceptable.");
}

