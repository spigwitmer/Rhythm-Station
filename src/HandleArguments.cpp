#include <cstring>
#include "HandleArguments.h"
#include "GameManager.h"
#include "Logger.h"

void HandleArguments(int argc, char** argv) {
	for (int i = 0; i<argc; i++) {
		if (!strcmp(argv[i], "--windowed")) {
			Log->Print("Windowed.");
			continue;
		}
		if (!strcmp(argv[i], "--fullscreen")) {
			Log->Print("Fullscreen.");
			continue;
		}
		if (!strcmp(argv[i], "--debug")) {
			Game->SetDebugMode(true);
			Log->Print("Debug mode.");
			continue;
		}
	}
}
