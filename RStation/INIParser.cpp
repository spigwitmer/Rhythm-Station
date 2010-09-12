#include "IniParser.h"
#include "FileManager.h"

using namespace std;

vector<string> split(string &str, char delim)
{
	vector<string> elems;
	stringstream ss(str);
	string item;
	while(getline(ss, item, delim))
		elems.push_back(item);
	return elems;
}

void IniParser::Load(string _path)
{
	string current_section = "";
	_path = FileManager::GetFile(_path);
	string file_contents = FileManager::GetFileContents(_path);

	vector<string> lines = split(file_contents, '\n');
	for(unsigned i = 0; i<lines.size(); i++)
	{
		string line = lines[i];
		if(line[0] == '[' && line[line.size()-1] == ']')
			current_section = line.substr(1,line.size()-2);
		if(!current_section.empty())
		{
			size_t pos;
			pos = line.find("=");
			if(pos == string::npos)
				continue;
			string key = line.substr(0,pos);
			string value = line.substr(pos+1,line.size()-1);
			
			Log::Print("Key: " + key + ", Value: " + value);
			
			ini_data[current_section].insert(pair<string,string>(key, value));
		}
	}
}

//template <class T>
string IniParser::getValue(string section, string key)
{
	if(!ini_data[section].empty())
	{
		if(ini_data[section].find(key) != ini_data[section].end())
			return ini_data[section].find(key)->second;
	}
	return string();
}
