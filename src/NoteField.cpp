#include "NoteField.h"
#include "Logger.h"

NoteField::NoteField()
{
	mColumns = 4;
}

NoteField::~NoteField()
{
	// TODO
}

void NoteField::setColumns(int cols)
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

int wrap(int a, int b)
{
	while (a>b)
		a -= b;
	return a;
}

void NoteField::Load(std::string path)
{
	float bpm = 140.0f;
	for (int i = 0; i<10; i++) {
		NoteRow row;
		row.scroll_speed = bpm;
		row.time = (1.f/bpm) * i * 1000;
		
		Note note;
		note.column = wrap(i, mColumns);
		note.type = Note_Tap;
		
		row.notes.push_back(note);
		
		mChart.note_rows.push_back(row);
	}
	this->Print();
}

void NoteField::Print()
{
	for (size_t i = 0; i<mChart.note_rows.size(); i++) {
		NoteRow currow = mChart.note_rows[i];
		Log->Print("Notes at time: %0.3fs:", float(currow.time/1000.f));
		for (size_t j = 0; j<currow.notes.size(); j++) {
			Note curnote = currow.notes[j];
			Log->Print("%d in column %d",
				curnote.type+1, curnote.column);
		}
	}
}

void NoteField::Update(double delta)
{
	// TODO
}

void NoteField::Draw()
{
	// TODO
}
