#include "RStation.h"
#include "RSUtil.h"
#include <GL/glfw.h>
#include <sstream>
#include <stdio.h>

using namespace std;

void Util::UpdateWindowTitle(float delta)
{
	double fps = 1.f / delta;

	ostringstream str;
	str << "RStation - ";
	str << "FPS: ";
	str << int(fps * 10) * 0.1f;
	str << ", Delta: ";
	str << delta;

	string sfps = str.str(); // str.c_str doesn't work?
	glfwSetWindowTitle(sfps.c_str());
}

vector<string> Util::split(string &str, char delim)
{
	vector<string> elems;
	stringstream ss(str);
	string item;
	while (getline(ss, item, delim))
		elems.push_back(item);
	return elems;
}

string Util::chop(string str, string search)
{
	size_t pos = str.find(search);
	if (pos != string::npos)
		str = str.substr(0,pos);
	return str;
}

// TODO: cleanup
void Util::trim(string& str)
{
	// remove spaces
	string::size_type pos = str.find_last_not_of(' ');
	if (pos != string::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if (pos != string::npos)
			str.erase(0, pos);
	}
	else
		str.erase(str.begin(), str.end());

	// and tabs
	pos = str.find_last_not_of('\t');
	if (pos != string::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of('\t');
		if (pos != string::npos)
			str.erase(0, pos);
	}
	else
		str.erase(str.begin(), str.end());
	
	// and newlines
	pos = str.find_last_not_of('\n');
	if (pos != string::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of('\n');
		if (pos != string::npos)
			str.erase(0, pos);
	}
	else
		str.erase(str.begin(), str.end());

}
