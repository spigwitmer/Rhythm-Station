#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include <map>

class Message
{
public:
	// Send message to all listeners
	void Send();

	std::string name;
	
	// XXX: not sure how well this works.
	std::map<std::string, void*> data;
};

class MessageListener
{
public:
	// Don't use pure virtual so that descended classes can omit these.
	// Might change later.
	virtual ~MessageListener() { } // silence warning
	virtual void HandleMessage(const Message &msg) { }
	virtual void SubscribeTo(std::string name);
};

#endif
