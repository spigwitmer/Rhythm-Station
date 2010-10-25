#ifndef _PNG_LOADER_H_
#define _PNG_LOADER_H_

#include "ResourceManager.h"

class PNGLoader {
public:
	void Load(std::string path);
	Texture im_texture;
};

#endif
