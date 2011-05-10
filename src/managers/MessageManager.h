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
