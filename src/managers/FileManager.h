#pragma once

#include <string>
#include <vector>

class FileManager
{
public:
	FileManager();
	virtual ~FileManager();
	void Mount(std::string internal, std::string real);
	bool FileExists(std::string path, std::string ext = "");
	
	std::vector<std::string> GetDirectoryListing(std::string path, std::string ext = "");
	std::string GetPath(std::string path);
	std::string GetFileContents(std::string path);
	
private:
	void SetWorkingDirectory();
	std::string GetWorkingDirectory();
};

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
