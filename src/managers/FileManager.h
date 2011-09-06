#pragma once

#include <string>
#include <vector>

class FileManager
{
public:
	FileManager();
	virtual ~FileManager();
	void Mount(std::string internal, std::string real);
	std::vector<std::string> GetDirectoryListing(std::string path);
};

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
