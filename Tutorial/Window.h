#pragma once

#include <Windows.h>

class Window
{
public:
	Window() = default;
	~Window() = default;

	bool Initialize();
	bool Broadcast();
	bool IsRunning();
	bool Release();

	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnDestroy();

protected:
	HWND m_hwnd = nullptr;
	bool m_is_running;
};

