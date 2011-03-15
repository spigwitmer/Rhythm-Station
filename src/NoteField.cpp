#include "NoteField.h"
#include "Logger.h"
#include "Sprite.h"
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include "GameManager.h"
#include "FileManager.h"

NoteField::NoteField()
{
	setColumns(4);
	setSpeed(1.0);
	
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
	if (mIsLoaded && !mStarted)
		onStart();
	
	if (mFinished)
		return;
	
	std::vector<NoteRow>::iterator row = mChart.note_rows.begin();
	
	unsigned int maxtime = 0;
	for ( ; row != mChart.note_rows.end(); row++) {
		maxtime = (row->time > maxtime) ? row->time : maxtime;
		// XXX: ...what.
		if (row->time > static_cast<unsigned int>(mTimer.Ago()*10) &&
			mTimer.Ago()*10 - row->time < 100)
			mValidRows.push_back(*row);
	}
	if (mTimer.Ago()*10 > maxtime)
		onFinish();
}

void NoteField::Draw()
{
	std::vector<NoteRow>::iterator row = mValidRows.begin();
	std::vector<Note>::iterator notes;
	
	mTexture.Bind();
	mShader.Bind();
	
	// Set ProjectionMatrix and Texture0. We'll need to keep updating ModelView.
	mShader.setUniforms();
	
	for ( ; row != mValidRows.end(); row++) {
		float scroll_speed = row->scroll_speed * mSpeed;
		float y = (((row->time-mChart.timing_offset)/10.f) - mTimer.Ago()) * scroll_speed;
		
		// _Basic_ (manual) view culling. StepMania uses a similar method.
		if (y > 250 || y < -200)
			continue;
		
		// Draw all the notes in this column
		for (notes = row->notes.begin(); notes != row->notes.end(); notes++) {
			float x = 0.0f;
			
			// Copy the parent transformation.
			if (!m_parent)
				m_matrix.Identity();
			else
				m_matrix = m_parent->getMatrix();
			
			// This should explain itself, a comment looks good here though.
			m_matrix.Translate(vec3((notes->column-(mColumns/2))*64.0f, y, 0));
			
			// Rotate if we have the numbers for it.
			if (mColumns == 4)
				m_matrix.Rotate(columnRotations[notes->column-1], 0, 0, 1);
			
			// Update color and transformation.
			glUniform4fv(mShader.getUniform("Color"), 1, glm::value_ptr(vec4(1.0, 1.0, 1.0, 1.0)));
			glUniformMatrix4fv(mShader.getUniform("ModelViewMatrix"), 1, false, glm::value_ptr(m_matrix.matrix));
			
			mMesh.Draw();
		}
	}
	
	// Yes game, we actually are doing something here.
	Game->QueueRendering();
}
