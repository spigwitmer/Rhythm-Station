#include "FileManager.h"
#include <glsw.h>
#include <string>

using namespace std;

FileManager::FileManager() {
	glswInit();
	glswSetPath("", ".glsl");
	
	/*
	 * Enforce 120 for GL 2.x, and 150 for 3.x.
	 * We only actually support GL 2.1 and 3.2, 
	 */
	glswAddDirectiveToken("GL21", "#version 120");		
	glswAddDirectiveToken("GL32", "#version 150");
}

FileManager::~FileManager() {
	glswShutdown();
}

void FileManager::Mount(string internal, string real)
{
	// TODO
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
