#ifndef DEVUI_H
#define DEVUI_H

#include <Config.hpp>

#include <functional>

namespace DevUI
{
	void Start();
	void Render();
	
	void HandleKeyEvent(int key, int scancode, int action, int mode);

	void RegisterOptionsFunc(std::function<void()> func);
}

#endif // DEVUI_H
