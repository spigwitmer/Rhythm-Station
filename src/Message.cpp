#include "Application.h"
#include "Message.h"

void Message::Send()
{
	Application* app = Application::getInstance();
	app->DispatchMessage(*this);
}
