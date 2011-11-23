#ifndef SCREEN_TEST_H
#define SCREEN_TEST_H

#include "Screen.h"
#include "managers/ScreenManager.h"
#include "utils/Message.h"

class ScreenTestDrawing : public Screen
{
public:
	ScreenTestDrawing(std::string sName);
	virtual ~ScreenTestDrawing();
	
	void Init() {}
	
	void UpdateInternal(double delta);
	void HandleMessage(const Message &msg);
	
	void Draw();
	
protected:
	const char *vss, *fss;
	unsigned /*vs, fs, id,*/ vao, buf[2], tex;
	std::string log;	
};

#endif
