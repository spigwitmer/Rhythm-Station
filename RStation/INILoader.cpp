#include "INILoader.h"
#include "FileManager.h"
#include "RSUtil.h"

using namespace std;
using namespace Util;

void INILoader::Load(string _path)
{
	string current_section = "";
	_path = File->GetFile(_path);
	string file_contents = File->GetFileContents(_path);

	vector<string> lines = split(file_contents, '\n');
	for (unsigned i = 0; i<lines.size(); i++)
	{
		string line = lines[i];
		
		// first-character comments
		switch (line[0])
		{
		case '#':
		case ';':
			continue;
		default:
			break;
		}
		
		// multi-character comments
		// // comment
		line = chop(line, "//");
		
		// -- comment
		line = chop(line, "--");

		// sections
		if (line[0] == '[' && line[line.size()-1] == ']')
			current_section = line.substr(1,line.size()-2);
		if (!current_section.empty())
		{
			// split key=value pairs and put them in our map
			size_t pos = line.find("=");
			if (pos == string::npos)
				continue;
			string key = line.substr(0,pos);
			string value = line.substr(pos+1,line.size()-1);

			ini_data[current_section].insert(pair<string,string>(key, value));
		}
	}
}

//template <class T>
string INILoader::getValue(string section, string key, string default_value)
{
	if (!ini_data[section].empty())
	{
		if (ini_data[section].find(key) != ini_data[section].end())
			return ini_data[section].find(key)->second;
	}
	return default_value;
}
