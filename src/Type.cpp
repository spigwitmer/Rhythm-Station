#include "Type.h"
#include "Logger.h"

std::string toString(int _in)
{
	std::stringstream out;
	out << _in;
	return out.str();
}

std::string toString(double _in)
{
	std::stringstream out;
	out << _in;
	return out.str();
}

std::string toString(bool _in)
{
	std::stringstream out;
	
	if ( _in )
		out << "true";
	else
		out << "false";
		
	return out.str();
}
