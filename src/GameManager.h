#ifndef _GAME_MANAGER_
#define _GAME_MANAGER_

#include <GL/glfw3.h>
#include <vector>
#include <map>
#include "InputManager.h"
#include "Object.h"
#include "Sound.h"
#include "Window.h"
#include "Matrix.h"
#include "Singleton.h"

class GameManager : public Singleton<GameManager>
{
public:
	GameManager(GLFWwindow window);
	virtual ~GameManager();
	
	void AddObject(Object *obj);
	void AddSound(Sound *snd);
	
	void Update(double delta);
	void QueueRendering();
	void Render();
	
	Matrix *ProjectionMatrix;
	
	std::map<std::string, bool> IsExtSupported;
	std::string GetExtensions();
	void CheckExtensions();
		
	double CurrentEqualizerFrame[20];
	
private:
	bool m_bFirstUpdate, m_bQueuedRender;
	
	std::vector<Object*> vpObjects;
	std::vector<Sound*> vpSounds;
};

extern GameManager *Game;

#endif
