#include <vector>
#include <string>
#include "RStation.h"

using namespace std;

int main(int argc, char **argv)
{
	vector<string> args;
	for (int i = 0; i<argc; i++)
	{
		args.push_back(string(argv[i]));
	}

	RStation rs;
	return rs.Start(args);
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
