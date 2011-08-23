#ifndef _SCREEN_MANAGER_H_
#define _SCREEN_MANAGER_H_

#include <vector>
#include <string>
#include <map>

#include "utils/Singleton.h"
#include "utils/Message.h"

class Screen;

// from Tanarux (thanks Matt)
typedef Screen* (*MakeScreenFn)(const std::string &sName);
typedef std::pair<std::string, MakeScreenFn> MakeScreenEntry;
typedef std::map<std::string, MakeScreenFn> MakeScreenMap;

class ScreenManager : public Singleton<ScreenManager>
{
public:
	void Update(double time);
	void Draw();
	
	void PushScreen(std::string sName);
	void PopScreen();
	void ClearStack();
	
	Screen *GetTopScreen() const { return m_vScreenStack.back(); }

	// Static map stuff, for registering
	static MakeScreenMap *GetMap();

private:
	double m_LastUpdate;

	std::vector<Screen*> m_vScreenStack;
	std::map<std::string, Screen*> m_vScreenTypes;
};

struct RegisterScreen
{
	RegisterScreen(const std::string &sName, MakeScreenFn fn)
	{
		ScreenManager::GetMap()->insert(MakeScreenEntry(sName, fn));
	}
};

#define REGISTER_SCREEN( name ) \
	Screen *Make##name( const string &sName ) { Screen *pRet = new name( sName ); pRet->Init(); return pRet; } \
	static RegisterScreen g_##name( #name, Make##name );

#endif

/**
 * @file
 * @author Matt Vandermeulen, Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
