#ifndef _PNG_LOADER_H_
#define _PNG_LOADER_H_

#include "managers/ResourceManager.h"

class PNGFile
{
public:
	Texture Load(std::string path);
};

#endif
