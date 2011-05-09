#ifndef _RSTATION_H_
#define _RSTATION_H_

#include <vector>
#include <string>

class RStation
{
public:
	RStation();
	virtual ~RStation();
	
	int Start(std::vector<std::string> vArguments);

private:
	int Loop();

	std::vector<std::string> m_vArgs;
};

#endif

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
