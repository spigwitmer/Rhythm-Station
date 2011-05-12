#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include "utils/Type.h"
#include <glm/glm.hpp>

typedef struct GLData
{
	// Basic stuff
	glm::vec3 pos;
	glm::vec3 nor;
	glm::vec2 tex;
/*
	// Bone info
	struct {
		uint32_t ID;
		float weight;
	} bone;
	
	// Texture info
	struct {
		uint32_t ID;
		uint32_t blend;

		// 2 vec3's instead of 3 vec2's so we can keep it at 8 attribs.
		glm::vec3 coords[2];
	} texture;
*/
};

enum HorizAlign {
	H_ALIGN_LEFT = 0,
	H_ALIGN_CENTER,
	H_ALIGN_RIGHT
};

enum VertAlign {
	V_ALIGN_TOP = 0,
	V_ALIGN_CENTER,
	V_ALIGN_BOTTOM
};

class Drawable
{
public:
	Drawable();
	virtual ~Drawable();

	void Push();

	void Update(double delta);
	void Draw();

	virtual void UpdateInternal(double delta) = 0;
	virtual void DrawInternal() = 0;

	void SetPosition(glm::vec3 v)	{ m_position = v; }
	void SetScale(glm::vec3 v)		{ m_scale = v; }
	void SetSize(glm::vec3 v)		{ m_size = v; }
	void SetRotation(glm::vec3 v)	{ m_rotation = v; }

	glm::vec3 GetPosition() const	{ return m_size; }
	glm::vec3 GetScale() const		{ return m_scale; }
	glm::vec3 GetSize() const		{ return m_size; }
	glm::vec3 GetRotation() const	{ return m_rotation; }

protected:
	glm::vec3 m_position, m_scale, m_size, m_rotation;
	glm::mat4 m_matrix;

	// Our alignments
	HorizAlign m_halign;
	VertAlign m_valign;

	// vertices + indices
	unsigned m_VBO[2];
};

#endif

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
