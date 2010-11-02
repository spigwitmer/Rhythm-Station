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

#include <SLB/SLB.hpp>
void Type_Binding() {
	SLB::Class<vec3>("vec3")
       .constructor<float, float, float>()
       .set("normalize", &vec3::normalize)
       .set("flip", &vec3::flip)
       .set("dot", &vec3::dot)
       .set("cross", &vec3::cross)
       .set("length", &vec3::length)
	.set("x", &vec3::x)
	.set("y", &vec3::y)
	.set("z", &vec3::z);

	Log->Print("Registered vec3 class");
}
