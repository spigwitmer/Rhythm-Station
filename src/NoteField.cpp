#include "NoteField.h"
#include "Logger.h"

NoteField::NoteField()
{
	setColumns(4);
	setSpeed(1.0);
	
	mIsLoaded = mStarted = false;
}

// Nothing to do yet.
NoteField::~NoteField() { }

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

static int wrap(int a, int b)
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
		note.column = wrap(i+1, mColumns);
		note.type = Note_Tap;
		
		row.notes.push_back(note);
		
		mChart.note_rows.push_back(row);
	}
	mIsLoaded = true;
}

/*
void NoteField::Print()
{
	for (size_t i = 0; i<mChart.note_rows.size(); i++) {
		NoteRow currow = mChart.note_rows[i];
		Log->Print("Row at %0.3fs:", float(currow.time/1000.f));
		for (size_t j = 0; j<currow.notes.size(); j++) {
			Note curnote = currow.notes[j];
			Log->Print("- Type %d in column %d",
				curnote.type, curnote.column);
		}
	}
}
*/

void NoteField::onStart()
{
	mTimer.Touch();
	mStarted = true;
	
	Message msg;
	msg.name = "SongStarted";
	msg.Send();
	
	this->Update(0.0);
}

void NoteField::onFinish()
{
	Message msg;
	msg.name = "SongFinished";
	msg.Send();
	
	Log->Print("Song finished in %s", mTimer.strAgo().c_str());
}

void NoteField::Update(double delta)
{
	if (mIsLoaded && !mStarted)
		onStart();
	
	std::vector<Object*>::iterator it = mObjects.begin();
	for ( ; it != mObjects.end(); it++)
		(*it)->Update(delta);
}

void NoteField::Draw()
{
	std::vector<Object*>::iterator it = mObjects.begin();
	for ( ; it != mObjects.end(); it++)
		(*it)->Draw();
}
