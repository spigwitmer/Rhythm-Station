#include "MessageManager.h"
#include "Message.h"

void Message::Send()
{
	MessageManager *sender = MessageManager::getSingleton();
	sender->DispatchMessage(*this);
}
