#include <cstring>
#include "HandleArguments.h"
#include "GameManager.h"
#include "RenderManager.h"
#include "Logger.h"

void HandleArguments(int argc, char** argv)
{
	for (int i = 0; i<argc; i++)
	{
		if (!strcmp(argv[i], "--windowed"))
		{
			Log->Print("Windowed.");
			continue;
		}
		if (!strcmp(argv[i], "--fullscreen"))
		{
			Log->Print("Fullscreen.");
			continue;
		}
		if (!strcmp(argv[i], "--debug"))
		{
			Game->SetDebugMode(true);
			Log->Print("Debug mode.");
			continue;
		}
		if (!strcmp(argv[i], "--show-extensions"))
		{
			Log->Print("Available OpenGL extensions: \n%s",Renderer->GetExtensions().c_str());
			continue;
		}
	}
}
