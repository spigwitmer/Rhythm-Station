#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <string>
#include <vector>
#include <map>

#include "Message.h"
#include "Screen.h"
#include "Singleton.h"

class Application : public Singleton<Application>
{
public:
	// Initial game load.
	void Init();

	// Enter game loop
	int Run();

	// Messages
	void StartListening(std::string event, MessageListener* scr);
	void StopListening(std::string event, MessageListener* scr);

	void DispatchMessage(Message &msg);

	// Update game logic.
	void Update(double delta);

private:
	std::map<std::string, std::vector<MessageListener*> > vpEventListeners;

	std::vector<Screen*> vpScreens;
};

#endif
