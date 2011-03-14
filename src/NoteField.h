#ifndef _NOTE_FIELD_H_
#define _NOTE_FIELD_H_

#include "Object.h"
#include "Sprite.h"
#include "TimingData.h"
#include "Mesh.h"

class NoteField : public Object
{
public:
	NoteField();
	virtual ~NoteField();
	
	void Load(std::string path);	
	
	void Update(double delta);
	void Draw();
	
	void onStart();
	void onFinish();
	
	// Debug.
	void Print();
	
	void setColumns(int cols);
	void setNoteskin(std::string name);
	void setSpeed(float multiplier);
	
private:
	std::string mNoteskin;
	bool mIsLoaded, mStarted, mFinished;
	int mColumns;
	
	float mSpeed;
	
	unsigned long mPosition;
	unsigned long mOffset;
	
	// Step/Chart data.
	Timer mTimer;
	TimingData mChart;
	std::vector<NoteRow> mValidRows;
	Matrix mMatrix;
	Shader mShader;
	
	// Secretly, all our note drawing is the same one being thrown around.
	Mesh mMesh;
};

#endif
