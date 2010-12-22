#include "Screen.h"
#include "Timer.h"
#include "TimingData.h"
#include <vector>

#ifndef _SCREEN_GAMEPLAY_H_
#define _SCREEN_GAMEPLAY_H_

class ScreenGameplay : public Screen
{
public:
	ScreenGameplay();
	virtual ~ScreenGameplay();

	void Update(double delta);
	void Draw();

private:
	// should have more info than just timingdata.
	std::vector<TimingData> vNoteData;
};

#endif
