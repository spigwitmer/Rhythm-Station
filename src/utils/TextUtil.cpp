#include "TextUtil.h"

using namespace std;

// Split function from other Reenigne projects
void TextUtil::Split(const string &s, const char c, vector<string> &add, int iLimit)
{
	// Split it into words
	unsigned iPos = 0, iLastPos = 0;

	// Test the limit, we can't logistically be more than the length
	if (iLimit <= 0)
		iLimit = s.length();

	// while we haven't hit the ass end..
	for (int i = 1 ; i <= iLimit || iPos < s.length() ; i++)
	{
		// Get the next space
		iPos = s.find_first_of(c, iLastPos);

		// If it's out of range, or hit limit?
		if (iPos >= s.length() || i == iLimit)
		{
			add.push_back(s.substr(iLastPos, s.length()));
			break;
		}

		// Add the word, reset position
		add.push_back(s.substr(iLastPos, iPos - iLastPos));
		iLastPos = ++iPos;
	}
}

