#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_

#include <string>
#include <map>
#include "utils/Singleton.h"
#include "utils/ByteBuffer.h"

// FileManager handles the virtual filesystem used by the game.
class FileManger : public Singleton<FileManager>
{
public:
	void Mount(std::string vfspath, std::string realpath)
	ByteBuffer Get(std::string path);
private:
	std::map<std::string, std::string> m_paths;
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
