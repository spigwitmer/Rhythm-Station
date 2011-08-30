#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <string>
#include <glm/glm.hpp>
#include "drawables/Drawable.h"
#include "managers/ScreenManager.h"
#include "utils/Message.h"

class Screen : public MessageListener
{
public:
	Screen(std::string sName);
	virtual ~Screen();

	void HandleMessage(const Message &msg);

	void Update(double delta);
	void Draw();

	void Push(Drawable* obj);

	std::string GetName() const { return m_name; }
	glm::mat4 *GetProjection() { return &m_projection; }

	// For derived classes only.
	virtual void Init();
	virtual void UpdateInternal(double delta);

protected:
	std::string m_name;
	glm::mat4 m_projection;

	std::vector<Drawable*> m_objects;
};

#endif

/**
 * Colby Klein, Matt Vandermeulen (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
