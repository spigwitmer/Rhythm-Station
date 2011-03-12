#include "NoteField.h"
#include "Logger.h"
#include <cmath>

NoteField::NoteField()
{
	setColumns(4);
	setSpeed(1.0);
	
	mIsLoaded = mStarted = mFinished = false;
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
		row.time = (1.f/bpm) * 1000 * i;
		
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
}

void NoteField::onFinish()
{
	Message msg;
	msg.name = "SongFinished";
	msg.Send();
	
	Log->Print("Chart finished in %0.2fs.", mTimer.Ago());
	
	mFinished = true;
}

void NoteField::Update(double delta)
{
	if (mIsLoaded && !mStarted)
		onStart();
	
	if (mFinished)
		return;
	
	std::vector<NoteRow> valid_rows;
	std::vector<NoteRow>::iterator row = mChart.note_rows.begin();
	std::vector<std::pair<NoteRow, Object*> >::iterator obj;
	
	unsigned int maxtime = 0;
	for ( ; row != mChart.note_rows.end(); row++) {
		maxtime = (row->time > maxtime) ? row->time : maxtime;
		/* ...wtf? 10x scale difference. */
		if (row->time > static_cast<unsigned int>(mTimer.Ago()*10)) {
			for (size_t j = 0; j < row->notes.size(); j++) {
//				Log->Print("Does it work? @ %0.2f", row->time/10.f);
			}
		}
	}
	if (mTimer.Ago()*10 > maxtime)
		onFinish();

	for (obj = mObjects.begin(); obj != mObjects.end(); obj++)
		obj->second->Update(delta);
}

void NoteField::Draw()
{
	std::vector<std::pair<NoteRow, Object*> >::iterator obj;
	for (obj = mObjects.begin() ; obj != mObjects.end(); obj++)
		obj->second->Draw();
}
