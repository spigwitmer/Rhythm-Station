#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

#include <string>
#include <map>

class RenderManager {
public:
	RenderManager();
	virtual ~RenderManager() { }

	std::string GetExtensions();
	std::map<std::string, bool> IsExtSupported;
private:
	void CheckExtensions();
};

extern RenderManager* Renderer;

#endif
