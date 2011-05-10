#include "InputManager.h"
#include "utils/Logger.h"

#include <vector>
#include <GL/glfw3.h>

#include "input/InputExtras.h"

// Our internal list of buttons
static ButtonState *g_pButtons[RS_INPUT_MAX];

// Our static queue, and its pointer
static std::vector<ButtonState*> g_vQueue;
static unsigned g_iQueuePtr = 0;

// The mouse position
static glm::ivec2 g_iMousePos;

// Define our callbacks
static void KeyboardCallback(GLFWwindow, int, int);
static void MouseButtonCallback(GLFWwindow, int, int);
static void MousePosCallback(GLFWwindow, int, int);
static void MouseScrollCallback(GLFWwindow, int, int);

InputManager::InputManager()
{
	// Initialize all our buttons
	for (unsigned i = 0; i < RS_INPUT_MAX; i++)
		g_pButtons[i] = new ButtonState( i );
	
	// Reserve 16 slots in the queue initially
	g_vQueue.reserve(16);
	
	// Set the callbacks up
	glfwSetKeyCallback(KeyboardCallback);
	glfwSetMouseButtonCallback(MouseButtonCallback);
	glfwSetMousePosCallback(MousePosCallback);
	glfwSetScrollCallback(MouseScrollCallback);
}

InputManager::~InputManager()
{
	// Unset the callbacks
	glfwSetKeyCallback( NULL );
	glfwSetMouseButtonCallback( NULL );
	glfwSetMousePosCallback( NULL );
	glfwSetScrollCallback( NULL );
	
	// Clear our queue
	DiscardQueue();
	
	// Deallocate the buttons
	for (unsigned i = 0; i < RS_INPUT_MAX; i++)
	{
		delete g_pButtons[i];
		g_pButtons[i] = NULL;
	}
}

void InputManager::DiscardQueue()
{
	// Clear it
	g_vQueue.clear();
	
	// Reset the pointer
	g_iQueuePtr = 0;
}

void InputManager::Update()
{
	// Discard the queue initially
	DiscardQueue();
	
	// Poll updates from glfw
	glfwPollEvents();
}

ButtonState *InputManager::GetNextInput()
{
	// Is there anything in the queue?
	if (g_vQueue.empty())
		return NULL;
	
	// Test the queue pointer
	if (g_iQueuePtr >= g_vQueue.size())
		return NULL;
	
	// Return the button
	return g_vQueue[g_iQueuePtr++];
}

ButtonState *InputManager::GetButton(RSButtons b) const
{
	// Sanity!
	if (b >= RS_INPUT_MAX)
		return NULL;
	
	// Return the button
	return g_pButtons[b];
}

glm::ivec2 &InputManager::GetMousePos() const
{
	// Just get the position
	return g_iMousePos;
}

static void KeyboardCallback(GLFWwindow, int iButton, int iDown)
{
	// Translate the button
	RSButtons b = TranslateGLFW(iButton);
	
	// Test it
	if (b == RS_KEY_INVALID)
	{
		LOG->Warn("[Input] Unknown GLFW Key '%i'", iButton);
		return;
	}
	
	// Now press it accordingly
	g_pButtons[b]->Press(!!iDown);
	
	// Add this button to the queue
	g_vQueue.push_back(g_pButtons[b]);
}

static void MouseButtonCallback(GLFWwindow, int iButton, int iDown)
{
	// Translate the button
	RSButtons b = TranslateGLFW(iButton);
	
	// Test it
	if (b == RS_KEY_INVALID)
	{
		LOG->Warn("[Input] Unknown GLFW Mouse button '%i'", iButton);
		return;
	}
	
	// Now press it accordingly
	g_pButtons[b]->Press(!!iDown);
	
	// Add this button to the queue
	g_vQueue.push_back(g_pButtons[b]);
}

static void MousePosCallback(GLFWwindow, int iPosX, int iPosY)
{
	// This is simple, just set stuff
	g_iMousePos.x = iPosX;
	g_iMousePos.y = iPosY;
}

static void MouseScrollCallback(GLFWwindow, int iHorizontal, int iVertical)
{
	/* Okay, we will press it, and add it to the queue...But it doesn't
	 * get unpressed */
	if (iHorizontal > 0)
	{
		g_pButtons[RS_SCROLL_RIGHT]->Press(true);
		g_vQueue.push_back(g_pButtons[RS_SCROLL_RIGHT]);
	}
	else if (iHorizontal < 0)
	{
		g_pButtons[RS_SCROLL_LEFT]->Press(true);
		g_vQueue.push_back(g_pButtons[RS_SCROLL_LEFT]);
	}
	
	if (iVertical > 0)
	{
		g_pButtons[RS_SCROLL_UP]->Press(true);
		g_vQueue.push_back(g_pButtons[RS_SCROLL_UP]);
	}
	else if (iVertical < 0)
	{
		g_pButtons[RS_SCROLL_DOWN]->Press(true);
		g_vQueue.push_back(g_pButtons[RS_SCROLL_DOWN]);
	}
}
