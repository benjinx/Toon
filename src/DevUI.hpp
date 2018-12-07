#ifndef DEVUI_H
#define DEVUI_H

#include "Common.hpp"

#include <vector>
#include <functional>

namespace DevUI
{
	void Start();
	void Render();
	void HandleEvent(int key);

	void RegisterOptionsFunc(std::function<void()> func);
}

#endif // DEVUI_H
