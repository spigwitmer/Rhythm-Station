#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include <map>

struct Message
{
	std::string name;
	std::map<std::string, void *> data;
	
	// send message to all listeners
	void Send();
};

struct MessageListener
{
	// Don't use pure virtual so that descended classes can omit these.
	// Might change later.
	virtual ~MessageListener() { } // silence warning
	virtual void HandleMessage(const Message &msg) { }
};

#endif
