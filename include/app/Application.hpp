#pragma once

#include "windows.hpp"


class Application {
	
public:
	Application();
	~Application();

	void Run();

private:
	Windows m_windows;
	bool g_Running = true;
};