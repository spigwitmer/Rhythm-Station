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
	
	void setColumns(char cols);
	void setNoteskin(std::string name);
	void setSpeed(float multiplier);
	
private:
	// Noteskin to load/store
	std::string mNoteskin;
	
	// Number of columns
	char mColumns;
	
	// Scroll rate multiplier
	float mSpeed;
	
	// Song position
	double mPosition, mOffset;
	
	// Step/Chart data.
	TimingData mChart;
};

#endif
