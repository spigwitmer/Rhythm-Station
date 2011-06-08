#ifndef TEXT_UTIL_H
#define TEXT_UTIL_H

#include <string>
#include <vector>

namespace TextUtil
{
	void Split(const std::string &s, const char c, std::vector<std::string> &vAdd, int iLimit = 0);
}

#endif // TEXT_UTIL_H

