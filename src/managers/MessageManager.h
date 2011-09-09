#ifndef _MESSAGE_MANAGER_H_
#define _MESSAGE_MANAGER_H_

#include "utils/Message.h"
#include "utils/Singleton.h"
#include <string>
#include <list>
#include <map>

class MessageManager : public Singleton<MessageManager>
{
public:
	virtual ~MessageManager();
	
	void Broadcast(Message &m);
	void Subscribe(MessageListener* listener, std::string event);
	void Unsubscribe(MessageListener* listener, std::string event);
	void RemoveListener(MessageListener* listener);
	void Clear();
	
protected:
	std::map<std::string, std::list<MessageListener*> > m_listeners;
};

#endif

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
