#ifndef _SCREEN_MANAGER_H_
#define _SCREEN_MANAGER_H_

#include <deque>
#include <vector>
#include <string>
#include <map>

#include "managers/LuaManager.h"
#include "utils/Message.h"

class Screen;

// from Tanarux (thanks Matt)
typedef Screen* (*MakeScreenFn)(const std::string &sName);
typedef std::pair<std::string, MakeScreenFn> MakeScreenEntry;
typedef std::map<std::string, MakeScreenFn> MakeScreenMap;

class ScreenManager
{
public:
	ScreenManager(LuaManager &lua);
	
	void Update();
	void Draw();
	
	void PushScreen(std::string sName);
	void PopScreen();
	void ClearStack();
	
	Screen *GetTopScreen() const { return m_vScreenStack.back(); }

	// Static map stuff, for registering
	static MakeScreenMap *GetMap();

private:
	double m_LastUpdate;
	long m_LastUpdateRounded;
	std::deque<double> m_Times;

	std::vector<Screen*> m_vScreenStack;
	std::map<std::string, Screen*> m_vScreenTypes;
	
	LuaManager *m_LuaMan;
};

struct RegisterScreen
{
	RegisterScreen(const std::string &sName, MakeScreenFn fn)
	{
		ScreenManager::GetMap()->insert(MakeScreenEntry(sName, fn));
	}
};

#define REGISTER_SCREEN( name ) \
	Screen *Make##name( const string &sName ) { Screen *pRet = new name( sName ); return pRet; } \
	static RegisterScreen g_##name( #name, Make##name );

#endif

/**
 * Colby Klein, Matt Vandermeulen (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
