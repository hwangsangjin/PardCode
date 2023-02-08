#pragma once

#include <Windows.h>

class Window
{
public:
	Window() = default;
	~Window() = default;

	bool Init();
	bool Broadcast();
	bool IsRun();
	bool Release();

	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDestroy();

protected:
	HWND m_hwnd;
	bool m_is_run;
};

