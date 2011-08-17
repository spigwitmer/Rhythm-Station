#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Drawable.h"
#include "managers/ScreenManager.h"
#include "screens/Screen.h"

Drawable::Drawable() :
	m_position(0.0), m_scale(1.0), m_size(1.0), m_rotation(0.0),
	m_matrix(1.0), m_halign(H_ALIGN_LEFT), m_valign(V_ALIGN_TOP)
{
}

Drawable::~Drawable()
{
}

void Drawable::Push()
{
	ScreenManager::GetSingleton()->GetTopScreen()->Push(this);
}

void Drawable::Update(double delta)
{
}

void Drawable::Draw()
{
	glm::vec3 size = m_size * m_scale;	
	glm::vec3 pos = m_position;
	
	// Base the horizontal on alignment
	switch(m_halign)
	{
		case H_ALIGN_CENTER: pos.x -= size.x / 2; break;
		case H_ALIGN_RIGHT: pos.x -= size.x; break;
		default: break;
	}

	switch(m_valign)
	{
		case V_ALIGN_CENTER: pos.y -= size.y / 2; break;
		case V_ALIGN_BOTTOM: pos.y -= size.y; break;
		default: break;
	}
	
	glm::mat4 xlate = glm::translate(glm::mat4(1.0), pos);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), size);

	glm::mat4 rotate(1.0f);
	rotate = glm::rotate(rotate, m_rotation.x, glm::vec3(1.0, 0.0, 0.0));
	rotate = glm::rotate(rotate, m_rotation.y, glm::vec3(0.0, 1.0, 0.0));
	rotate = glm::rotate(rotate, m_rotation.z, glm::vec3(0.0, 0.0, 1.0));

	m_matrix = xlate * rotate * scale;

	DrawInternal();
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
