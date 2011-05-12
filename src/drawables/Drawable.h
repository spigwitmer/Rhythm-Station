#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include "utils/Type.h"
#include <glm/glm.hpp>

// Note: Exactly 64 bytes and 8 attributes.
typedef struct GLData
{
	// Basic stuff
	glm::vec3 pos;	// 3
	glm::vec3 nor;	// 6

	// Bone info
	struct {
		uint32_t ID; // 7
		float weight; // 8
	} bone;
	
	// Texture info
	struct {
		uint32_t ID;	// 9
		uint32_t blend;	// 10

		// 2 vec3's instead of 3 vec2's so we can keep it at 8 attribs.
		glm::vec3 coords[2]; // 16
	} texture;
};

class Drawable
{
public:
	Drawable();
	virtual ~Drawable();

	void Update(double delta);
	void Draw();

	virtual void UpdateInternal(double delta) = 0;
	virtual void DrawInternal() = 0;

protected:
	glm::vec3 m_position, m_scale, m_size, m_rotation;

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
