#include "Message.h"
#include "managers/MessageManager.h"

MessageListener::~MessageListener() {

}

void MessageListener::HandleMessage(const Message &msg)
{

}

void MessageListener::SubscribeTo(std::string name)
{
	MessageManager* m = MessageManager::GetSingleton();
	m->Subscribe(this, name);
}

/**
 * Colby Klein (c) 2011
 * Licensed under the terms of the MIT license. See License.txt.
 */
