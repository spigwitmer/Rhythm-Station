#ifndef _RSTATION_H_
#define _RSTATION_H_

#include <vector>
#include <string>

// Helps prevent heap corruption.
#define SAFE_DELETE(p) if( (p) != NULL ) delete (p); (p) = NULL;

class RStation
{
public:
	RStation(std::vector<std::string> &vArguments);
	virtual ~RStation();

	int Run();

private:
	std::vector<std::string> m_vArgs;
};

#endif

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
