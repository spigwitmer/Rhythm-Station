#ifndef _RSTATION_H_
#define _RSTATION_H_

#ifdef _WIN32
#pragma once
#endif

#ifdef _WIN32
#include "win32\Resource.h"
#include "win32\stdafx.h"
#pragma comment(lib,"glew32s.lib")
#pragma comment(lib,"GLFW.lib")
#pragma comment(lib,"libpng.lib")
#endif

#include <cstdlib>
#include <vector>
#include "Log.h"
#include "Type.h"

extern float g_aspectRatio;
extern bool g_mouse_detached;

#endif