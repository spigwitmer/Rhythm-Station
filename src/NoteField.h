#ifndef _NOTE_FIELD_H_
#define _NOTE_FIELD_H_

#include "Object.h"
#include "TimingData.h"

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
	TimingData mChart;
	Timer mTimer;
	
	// Notes
	std::vector<std::pair<NoteRow, Object*> > mObjects;
};

#endif
