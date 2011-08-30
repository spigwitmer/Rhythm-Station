#include <vector>
#include <string>
#include "RStation.h"

using namespace std;

int main(int argc, char **argv)
{
	// Build arguments into a vector before passing control
	vector<string> args;
	for (int i = 0; i<argc; i++)
	{
		args.push_back(string(argv[i]));
	}

	RStation rs(args);
	return rs.Run();
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
