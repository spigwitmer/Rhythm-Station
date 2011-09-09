#include "MessageManager.h"
#include "utils/Logger.h"

using namespace std;

MessageManager::MessageManager()
{
	// TODO
}

MessageManager::~MessageManager()
{
	// TODO
}

void MessageManager::Broadcast(Message &m)
{
	
}

void MessageManager::Subscribe(MessageListener *listener, string event)
{
	m_listeners[event].push_back(listener);
	LOG->Info("Subscribed to %s", event.c_str());
}

void MessageManager::Unsubscribe(MessageListener *listener, string event)
{
	/*
	 * Note: Frequent addition/removal of items in random positions is expected,
	 * therefore std::list is likely the best option here.
	 */
	list<MessageListener*>::iterator it;
	
	for (it = m_listeners[event].begin(); it != m_listeners[event].end(); it++)
	{
		if (*it == listener)
			m_listeners[event].erase(it);
	}
}

void MessageManager::RemoveListener(MessageListener *listener)
{
	map<string, list<MessageListener*> >::iterator event;
	list<MessageListener*>::iterator it;
	
	for (event = m_listeners.begin(); event != m_listeners.end(); event++)
	{
		for (it =  event->second.begin(); it != event->second.end(); it++)
		{
			if (*it == listener)
				event->second.erase(it);
		}
	}
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
