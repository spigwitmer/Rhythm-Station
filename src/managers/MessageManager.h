#ifndef _MESSAGE_MANAGER_H_
#define _MESSAGE_MANAGER_H_


#include "utils/Singleton.h"
#include "utils/Message.h"
#include <string>
#include <vector>
#include <map>

class MessageManager : public Singleton<MessageManager>
{
public:
	void StartListening(std::string event, MessageListener *scr);
	void StopListening(std::string event, MessageListener *scr);
	
	void DispatchMessage(Message &msg);
	
private:
	std::map<std::string, std::vector<MessageListener *> > vpEventListeners;
};

#endif

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * This program is licensed under the terms of the MIT license.
 * The full text can be found in License.txt.
 */
