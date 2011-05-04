#ifndef _NOTE_FIELD_H_
#define _NOTE_FIELD_H_

#include "Object.h"
#include "TimingData.h"

#include "renderer/Mesh.h"
#include "renderer/Texture.h"

class NoteField : public Object
{
public:
	NoteField();
	virtual ~NoteField();
	
	void Load(std::string path);
	
	void Update(double delta);
	void Draw();
	
	void HandleMessage(const Message &msg);
	
	void OnStart();
	void OnFinish();
		
	void SetColumns(int cols);
	void SetNoteskin(std::string name);
	void SetSpeed(float multiplier);
	
	void SetMaxBeforeReceptors(int max);
	void SetMaxAfterReceptors(int max);

private:
	std::string mNoteskin;
	bool mIsLoaded, mStarted, mFinished;
	int mColumns, mMaxBeforeReceptors, mMaxAfterReceptors;
	unsigned int mMaxTime;
	
	float mSpeed;
		
	// Step/Chart data.
	Timer mTimer;
	TimingData mChart;
	struct {
		double time;
		std::vector<Note*> notes;
	} mCurrentRow;
	
	// Secretly, all our note drawing is the same one being thrown around.
	Mesh mMesh;
	Texture mTexture;
	Shader mShader;
};

void NoteField_Binding();

#endif
