#pragma once

#include "windows/windows.hpp"
#include "gui/AnxGui.hpp"


class Application {
	
public:
	Application();
	~Application();

	void Run();

private:
	Windows m_windows;
	AnxGui m_anxGui;

	bool g_Running = true;
};