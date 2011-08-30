#pragma once

#include <string>

class FileManager
{
public:
	FileManager();
	virtual ~FileManager();
	void Mount(std::string internal, std::string real);
};

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
