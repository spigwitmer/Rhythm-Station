#ifndef _FONT_H_
#define _FONT_H_

#include "Object.h"
#include <string>

/*
 * A simple class for loading bitmap fonts (monospace, currently)
 * TODO: Proportional width, ttf/otf support via freetype2.
 */

class Font : public Object
{
public:
	void Load(std::string);
	void Set(std::string);
	
private:
	std::string m_text;
};

#endif
