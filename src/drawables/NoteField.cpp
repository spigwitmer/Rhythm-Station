#include <GL/glew.h>
#include "NoteField.h"
#include "utils/Logger.h"
#include "Sprite.h"
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include "managers/GameManager.h"
#include "managers/FileManager.h"

// Long!
typedef std::vector<NoteRow>::iterator RowIterator;
typedef std::vector<Note>::iterator NoteIterator;

NoteField::NoteField() : mColumns(4), mSpeed(1.0)
{	
	// Set intentionally small to encourage setting it in-theme.
	SetMaxBeforeReceptors(350);
	SetMaxAfterReceptors(-90);
	
	mIsLoaded = mStarted = mFinished = false;
	
	float vertices[] = {
		-0.5*64, -0.5*64, 0, 0, 0, 0, 0, 0,
		-0.5*64,  0.5*64, 0, 0, 0, 0, 0, 1,
		 0.5*64, -0.5*64, 0, 0, 0, 0, 1, 0,
		 0.5*64,  0.5*64, 0, 0, 0, 0, 1, 1,
	};
	unsigned indices[] = {
		0, 1, 2,
		1, 2, 3
	};
	MeshData verts[4];
	memcpy(&verts[0].Position.x, vertices, sizeof(vertices));
	mMesh.Load(verts, indices, 4, 6);
	
	mShader.setModelViewMatrix(&m_matrix);
	mTexture.Load(FileManager::GetFile("Graphics/arrow.png"));
	
	this->SubscribeTo("Input");
}

float columnRotations[] = { 90, 0, 180, -90 };

// Nothing to do yet.
NoteField::~NoteField() { }

void NoteField::SetColumns(int cols)
{
	if (cols < 1)
		cols = 4; // default
	
	mColumns = cols;
}

void NoteField::SetNoteskin(std::string name)
{
	if (name.empty())
		name = "default";

	mNoteskin = name;
//	mTexture.Load(FileManager::GetFile(name + "Graphics/arrow.png"));
}

void NoteField::SetSpeed(float multiplier)
{
	mSpeed = (multiplier > 0.0f) ? multiplier : 1.0f;
}


void NoteField::SetMaxBeforeReceptors(int max)
{
	mMaxBeforeReceptors = max;
}

void NoteField::SetMaxAfterReceptors(int max)
{
	mMaxAfterReceptors = max;
}

static int wrap(int a, int b)
{
	while (a>b)
		a -= b;
	return a;
}

void NoteField::Load(std::string path)
{
	// TODO: search path for chart + song file
	
	mChart.timing_offset = -50;
	float bpm = 144.0f;
	for (int i = 0; i<200; i++) {
		NoteRow row;
		row.scroll_speed = bpm;
		row.time = (60.f/bpm) * 1000 * i * 4;
		
		Note note;
		note.column = wrap(i+1, mColumns);
		note.type = Note_Tap;
		
		row.notes.push_back(note);
		
		mChart.note_rows.push_back(row);
		
		mMaxTime = (row.time > mMaxTime) ? row.time : mMaxTime;
	}
	mIsLoaded = true;
	Log->Print("Loaded. (%zd rows, mIsLoaded = %d)", mChart.note_rows.size(), mIsLoaded);
}

void NoteField::OnStart()
{
	mTimer.Touch();
	mStarted = true;
	
	Message msg;
	msg.name = "SongStarted";
	msg.Send();
}

void NoteField::OnFinish()
{
	Message msg;
	msg.name = "SongFinished";
	msg.Send();
	
	Log->Print("Chart finished in %0.2fs.", mTimer.Ago());
	
	mFinished = true;
}

static bool within(float in, float limit)
{
	if (fabsf(in) <= limit)
		return true;
	return false;
}

static void judge_note(float timing)
{
	if (timing <= 0.0225)
		Log->Print("Fantastic!");
	else if (timing <= 0.055)
		Log->Print("Perfect");
	else if (timing <= 0.11)
		Log->Print("Great");
	else if (timing <= 0.22)
		Log->Print("Good");
	else
		Log->Print("Almost...");
}

void NoteField::HandleMessage(const Message &msg)
{
	if (msg.name != "Input")
		return;
	
	std::map<std::string, void*> evt;
	evt = msg.data;
	
	// ¡¡¡ Voodoo alert !!!
	IEvent* status;
	status = (IEvent*)evt["Event"];
	
	char key = 'A';
	
	if (status->last_type == KEY_LETGO)
		return;
	
	// Get difference (timestamp is relative to the game, timer to this)
	float timeDifference = status->timestamp[(int)key] - mTimer.Ago();
	
	// Subtract difference
	float now = status->timestamp[(int)key] - timeDifference;
	float then = (mCurrentRow.time - mChart.timing_offset) / 1000.f;
	
	// TODO: only handles hitting the row itself. Should judge *notes*
	float timing = then - now;
	if (within(timing, 0.5))
		judge_note(timing);
}

void NoteField::Update(double delta)
{
	this->UpdateTweens(delta);
	
	if (mIsLoaded && !mStarted)
		OnStart();
	
	if (mFinished)
		return;
	
	if (mTimer.Ago() * 1000 > mMaxTime)
		OnFinish();
	
	// Re-populate notes to judge on every update.
	mCurrentRow.notes.clear();
	for (RowIterator row = mChart.note_rows.begin(); row != mChart.note_rows.end(); row++)
	{
		float row_time = (row->time - mChart.timing_offset) / 1000.f;
		if (within(row_time - mTimer.Ago(), 0.5))
		{
			for (NoteIterator note = row->notes.begin(); note != row->notes.end(); note++)
			{
				if (!note->hit) {
					Note *cur = note.base();
					mCurrentRow.notes.push_back(cur);
				}
			}
			mCurrentRow.time = row->time;
		}
		if (row_time + 0.5 > mTimer.Ago())
			break;
	}
}

void NoteField::Draw()
{
	Matrix noteMatrix;
		
	mTexture.Bind();
	mShader.Bind();
	
	glUniform4fv(mShader.getUniform("Color"), 1, glm::value_ptr(m_color));
	
	if (mChart.note_rows.empty())
		return;
	
	for (RowIterator row = mChart.note_rows.begin(); row != mChart.note_rows.end(); row++)
	{
		float speed, row_time, position, y_pos;
		speed		= row->scroll_speed * mSpeed;
		row_time	= (row->time - mChart.timing_offset) / 1000.f;
		position	= row_time - mTimer.Ago();
		y_pos		= position * speed;
		
		// Simple, yet effective culling.
		if (y_pos > mMaxBeforeReceptors || y_pos < mMaxAfterReceptors)
			continue;
		
		// Draw all notes in this column
		for (NoteIterator notes = row->notes.begin(); notes != row->notes.end(); notes++)
		{
			noteMatrix.Identity();
			noteMatrix.Translate(vec3((notes->column-(mColumns/2))*64.0f, y_pos, 0));
			
			// TODO: Should be able to pass these values in (via Lua, even)
			if (mColumns == 4)
				noteMatrix.Rotate(columnRotations[notes->column-1], 0, 0, 1);
			
			// Update transformation.
			noteMatrix.matrix = m_matrix.matrix * noteMatrix.matrix;
			
			glUniformMatrix4fv(mShader.getUniform("ModelViewMatrix"), 1, false, glm::value_ptr(noteMatrix.matrix));
			
			mMesh.Draw();
		}
	}
	
	// Yes game, we actually are doing something here.
	Game->QueueRendering();
}

// Lua
#include <SLB/SLB.hpp>
void NoteField_Binding()
{
	SLB::Class<NoteField>("NoteField")
	.constructor()
	
	.set("SubscribeTo", &NoteField::SubscribeTo)
	
	// Actor stuff.
	.set("Register", &NoteField::Register)
	.set("addChild", &NoteField::AddChild)
	.set("addState", &NoteField::AddState)
	.set("setParent", &NoteField::SetParent)
	.set("setPosition", &NoteField::SetPosition)
	.set("setRotation", &NoteField::SetRotation)
	.set("setScale", &NoteField::SetScale)
	.set("Update", &NoteField::Update)
	.set("Draw", &NoteField::Draw)
	
	// NoteField specifics
	.set("setMaxBeforeReceptors", &NoteField::SetMaxBeforeReceptors)
	.set("setMaxAfterReceptors", &NoteField::SetMaxAfterReceptors)
	.set("setColumns", &NoteField::SetColumns)
	.set("setNoteskin", &NoteField::SetNoteskin)
	.set("setSpeed", &NoteField::SetSpeed)
	.set("Load", &NoteField::Load);
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
