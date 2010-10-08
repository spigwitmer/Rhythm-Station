#ifndef _RSUTIL_H_
#define _RSUTIL_H_

#include "RStation.h"

namespace Util
{
	void UpdateWindowTitle(float delta);
	std::vector<std::string> split(std::string &str, char delim);
	std::string chop(std::string str, std::string search);
	void trim(std::string& str);
}

#endif
