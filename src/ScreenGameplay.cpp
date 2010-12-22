#include "ScreenGameplay.h"
#include "GameManager.h"

ScreenGameplay::ScreenGameplay()
{
	// TODO
}

ScreenGameplay::~ScreenGameplay()
{
	// TODO
}

void ScreenGameplay::Update(double delta)
{
	Game->QueueRendering();
}

void ScreenGameplay::Draw()
{
	// TODO
}
