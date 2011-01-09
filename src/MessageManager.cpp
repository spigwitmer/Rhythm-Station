#include "MessageManager.h"

void MessageManager::StartListening(std::string event, MessageListener* scr)
{
	vpEventListeners[event].push_back(scr);
}

void MessageManager::StopListening(std::string event, MessageListener* scr)
{
	// cut down some typing.
	std::vector<MessageListener*> listeners = vpEventListeners[event];
	
	// find the screen and remove it.
	for (size_t i = listeners.size(); i>0; --i)
	{
		if (listeners[i] == scr)
		{
			listeners.erase(listeners.begin()+i);
			break;
		}
	}
}

void MessageManager::DispatchMessage(Message &msg)
{
	std::vector<MessageListener*> listeners = vpEventListeners[msg.name];
	
	for (size_t i = 0; i<listeners.size(); i++)
		listeners[i]->HandleMessage(msg);
}
