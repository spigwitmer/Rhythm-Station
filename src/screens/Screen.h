#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <string>
#include <glm/glm.hpp>
#include "actors/Actor.h"
#include "managers/ScreenManager.h"
#include "managers/FileManager.h"
#include "managers/LuaManager.h"
#include "utils/Message.h"

class Screen : public MessageListener
{
public:
	// Set name on creation.
	Screen(std::string sName);
	virtual ~Screen();
	void Reset();

	void Update(double delta);
	virtual void Draw();

	// TODO
	void Push(Actor* obj);
	
	// Calls Init() on subclasses.
	void InitInternal();

	std::string GetName() const { return m_name; }
	glm::mat4 *GetProjection() { return &m_projection; }

	// For derived classes only.
	virtual void Init();
	virtual void UpdateInternal(double delta);
	
	virtual void HandleMessage(const Message &msg);

	void SetLuaManager(LuaManager *lm) { m_Lua = lm; }
	void SetFileManager(FileManager *fm) { m_FileMan = fm; }
	
protected:
	bool m_error;
	LuaManager *m_Lua;
	FileManager *m_FileMan;
	
	std::string m_name;
	glm::mat4 m_projection;

	std::vector<Actor*> m_objects;
};

#endif

/**
 * Colby Klein, Matt Vandermeulen (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
