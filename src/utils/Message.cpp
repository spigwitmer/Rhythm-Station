#include "managers/MessageManager.h"
#include "Message.h"

using namespace std;

void Message::Send()
{
	MessageManager *sender = MessageManager::GetSingleton();
	sender->DispatchMessage(*this);
}

void MessageListener::SubscribeTo(string name)
{
	MessageManager *msgman = MessageManager::GetSingleton();
	msgman->StartListening(name, this);
}

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
