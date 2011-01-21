#include "MessageManager.h"
#include "Message.h"

void Message::Send()
{
	MessageManager *sender = MessageManager::getInstance();
	sender->DispatchMessage(*this);
}
