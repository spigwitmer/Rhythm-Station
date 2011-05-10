#ifndef _RSTATION_H_
#define _RSTATION_H_

#include <vector>
#include <string>
#include "utils/Singleton.h"

// Helps prevent heap corruption.
#define SAFE_DELETE(p) if( (p) != NULL ) delete (p); (p) = NULL;

enum {
	RS_SUCCESS = 0,
	RS_INIT_FAILURE
};

class RStation : public Singleton<RStation>
{
public:
	RStation();
	virtual ~RStation();
	
	int Start(std::vector<std::string> vArguments);

private:
	int Loop();

	int m_status;
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
