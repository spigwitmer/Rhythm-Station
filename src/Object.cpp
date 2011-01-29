#include "Object.h"

Object::Object()
{
	
}

Object::~Object()
{
	
}

void Object::HandleMessage(std::string msg)
{
	
}

void Object::Update(double delta)
{
	
}

void Object::DrawPrimitives()
{
	
}

void Object::Draw()
{
	
}

// parenting... needs a getChild function still.
void Object::setParent(Object *obj)
{
	
}

void Object::addChild(Object *obj)
{
	
}

/*
void Object::getChildByName(const char *name)
{
	
}
*/

// animation
void Object::addState(int tweentype, double length)
{
	
}

void Object::setPosition(float x, float y, float z)
{
	
}

void Object::setRotation(float x, float y, float z)
{
	
}

void Object::setScale(float x, float y, float z)
{
	
}

Matrix Object::getMatrix()
{
	return m_matrix;
}

vec3 Object::getPosition()
{
	return m_position;
}

vec3 Object::getRotation()
{
	return m_rotation;
}

vec3 Object::getScale()
{
	return m_scale;
}
