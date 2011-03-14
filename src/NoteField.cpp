#include "NoteField.h"
#include "Logger.h"
#include "Sprite.h"
#include <cmath>
#include <glm/gtc/type_ptr.hpp>

NoteField::NoteField()
{
	setColumns(4);
	setSpeed(1.0);
	
	mIsLoaded = mStarted = mFinished = false;
	
	MeshData verts[4];
	float vertices[] = {
		-0.5, -0.5, 0, 0, 0, 0, 0, 0,
		-0.5,  0.5, 0, 0, 0, 0, 0, 1,
		0.5, -0.5, 0, 0, 0, 0, 1, 0,
		0.5,  0.5, 0, 0, 0, 0, 1, 1,
	};
	unsigned indices[] = {
		0, 1, 2,
		1, 2, 3
	};
	memcpy(&verts[0].Position.x, vertices, sizeof(vertices));
	mMesh.Load(verts, indices, 4, 6);
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
	
	std::vector<NoteRow>::iterator row = mChart.note_rows.begin();
	
	unsigned int maxtime = 0;
	for ( ; row != mChart.note_rows.end(); row++) {
		maxtime = (row->time > maxtime) ? row->time : maxtime;
		// XXX: ...what.
		if (row->time > static_cast<unsigned int>(mTimer.Ago()*10))
			mValidRows.push_back(*row);
	}
	if (mTimer.Ago()*10 > maxtime)
		onFinish();
}

void NoteField::Draw()
{
	float spacing = 64.0f;
	
	std::vector<NoteRow>::iterator row = mValidRows.begin();
	std::vector<Note>::iterator notes;
	for ( ; row != mValidRows.end(); row++) {
		for (notes = row->notes.begin(); notes != row->notes.end(); notes++) {
			mMatrix.Identity();
			mMatrix.Translate(vec3((notes->column-(mColumns/2))*spacing, ((row->time/10.f)*row->scroll_speed*mSpeed)-mTimer.Ago()*row->scroll_speed, 0));
			mMatrix.Scale(16, 16, 16);
			mShader.SetModelViewMatrix(&mMatrix);
			mShader.Bind();
			glUniform4fv(glGetUniformLocation(mShader.id, "Color"), 1, glm::value_ptr(vec4(1.0, 1.0, 1.0, 1.0)));
			glBindTexture(GL_TEXTURE_2D, 0);
			mMesh.Draw();
		}
	}
}
