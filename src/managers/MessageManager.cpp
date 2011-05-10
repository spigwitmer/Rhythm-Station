#include "MessageManager.h"

// I can't stand long types.
typedef std::vector<MessageListener*>::iterator MessageIterator;

void MessageManager::StartListening(std::string event, MessageListener *obj)
{
	vpEventListeners[event].push_back(obj);
}

void MessageManager::StopListening(std::string event, MessageListener *scr)
{
	std::vector<MessageListener *> listeners = vpEventListeners[event];

	// find the screen and remove it.
	for (MessageIterator it = listeners.begin(); it != listeners.end(); ++it)
	{
		if (*it == scr)
		{
			listeners.erase(it);
			break;
		}
	}
}

void MessageManager::DispatchMessage(Message &msg)
{
	std::vector<MessageListener*> listeners = vpEventListeners[msg.name];
	
	for (MessageIterator it = listeners.begin(); it != listeners.end(); ++it)
		(*it)->HandleMessage(msg);
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
