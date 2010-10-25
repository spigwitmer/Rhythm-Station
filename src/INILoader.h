#ifndef _INI_PARSER_H_
#define _INI_PARSER_H_

#include <string>
#include <map>

class INILoader
{
public:
	void Load(std::string path);
	std::string getKeys();
	std::string getValue(std::string key, std::string name, std::string default_value = "");
private:
	std::map<std::string, std::map<std::string, std::string> > ini_data;
};

#endif
