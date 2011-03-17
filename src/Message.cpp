#include "MessageManager.h"
#include "Message.h"

void Message::Send()
{
	MessageManager *sender = MessageManager::getSingleton();
	sender->DispatchMessage(*this);
}

void MessageListener::SubscribeTo(std::string name)
{
	MessageManager *msgman = MessageManager::getSingleton();
	msgman->StartListening(name, this);
}
