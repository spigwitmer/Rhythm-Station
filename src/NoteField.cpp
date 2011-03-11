#include "NoteField.h"

NoteField::NoteField()
{
	// TODO
}

NoteField::~NoteField()
{
	// TODO
}

void NoteField::setColumns(char cols)
{
	if (cols < 1)
		cols = 4; // default
	
	mColumns = cols;
}

void NoteField::setNoteskin(std::string name)
{
	if (name.empty())
		name = "default";

	mNoteskin = name;
}

void NoteField::setSpeed(float multiplier)
{
	mSpeed = (multiplier > 0.0f) ? multiplier : 1.0f;
}

void NoteField::Load(std::string path)
{
	// TODO
}
	
void NoteField::Update(double delta)
{
	// TODO
}

void NoteField::Draw()
{
	// TODO
}
