#pragma once

#ifdef _WIN32
#include "win32\Resource.h"
#include "win32\stdafx.h"
#pragma comment(lib,"glew32s.lib")
#pragma comment(lib,"GLFW.lib")
#pragma comment(lib,"libpng.lib")
#endif

#include <vector>
#include "Log.h"
#include "Type.h"
