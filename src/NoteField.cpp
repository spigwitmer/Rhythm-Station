#include "NoteField.h"
#include "Logger.h"
#include "Sprite.h"
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include "GameManager.h"
#include "FileManager.h"

// Long!
typedef std::vector<NoteRow>::iterator RowIterator;
typedef std::vector<Note>::iterator NoteIterator;

NoteField::NoteField() : mColumns(4), mSpeed(1.0)
{	
	m_position = vec3(0.0);
	m_color = vec4(1.0);
	m_scale = vec3(1.0);
	
	// Set intentionall small to encourage setting it in-theme.
	setMaxBeforeReceptors(350);
	setMaxAfterReceptors(-90);
	
	mIsLoaded = mStarted = mFinished = false;
	
	MeshData verts[4];
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
	memcpy(&verts[0].Position.x, vertices, sizeof(vertices));
	mMesh.Load(verts, indices, 4, 6);
	mShader.setModelViewMatrix(&m_matrix);
	mTexture.Load(FileManager::GetFile("Graphics/arrow.png"));
}

float columnRotations[] = { 90, 0, 180, -90 };

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
//	mTexture.Load(FileManager::GetFile(name + "Graphics/arrow.png"));
}

void NoteField::setSpeed(float multiplier)
{
	mSpeed = (multiplier > 0.0f) ? multiplier : 1.0f;
}


void NoteField::setMaxBeforeReceptors(int max)
{
	mMaxBeforeReceptors = max;
}

void NoteField::setMaxAfterReceptors(int max)
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
	
	mShader.Bind();
	mShader.getUniform("Color");
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
	this->UpdateTweens(delta);
	
	if (mIsLoaded && !mStarted)
		onStart();
	
	if (mFinished)
		return;
	
	// Work out the end time of the chart.
	unsigned int maxtime = 0;
	for (RowIterator row = mChart.note_rows.begin(); row != mChart.note_rows.end(); row++) {
		maxtime = (row->time > maxtime) ? row->time : maxtime;
	}
	if (mTimer.Ago()*10 > maxtime)
		onFinish();
}

void NoteField::Draw()
{
	Matrix noteMatrix;
		
	mTexture.Bind();
	mShader.Bind();
	
	glUniform4fv(mShader.getUniform("Color"), 1, glm::value_ptr(m_color));
	
	if (mChart.note_rows.empty())
		return;
	
	for (RowIterator row = mChart.note_rows.begin(); row != mChart.note_rows.end(); row++) {
		float speed, row_time, position, y_pos;
		speed		= row->scroll_speed * mSpeed;
		row_time	= (row->time - mChart.timing_offset) / 10.f;
		position	= row_time - mTimer.Ago();
		y_pos		= position * speed;
		
		// Simple, yet effective culling.
		if (y_pos > mMaxBeforeReceptors || y_pos < mMaxAfterReceptors)
			continue;
		
		// Draw all notes in this column
		for (NoteIterator notes = row->notes.begin(); notes != row->notes.end(); notes++) {
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
	.set("addChild", &NoteField::addChild)
	.set("addState", &NoteField::addState)
	.set("setParent", &NoteField::setParent)
	.set("setPosition", &NoteField::setPosition)
	.set("setRotation", &NoteField::setRotation)
	.set("setScale", &NoteField::setScale)
	
	// NoteField specifics
	.set("setMaxBeforeReceptors", &NoteField::setMaxBeforeReceptors)
	.set("setMaxAfterReceptors", &NoteField::setMaxAfterReceptors)
	.set("setColumns", &NoteField::setColumns)
	.set("setNoteskin", &NoteField::setNoteskin)
	.set("setSpeed", &NoteField::setSpeed)
	.set("Load", &NoteField::Load);
}
