#pragma once

#include <Windows.h>

class Window
{
public:
	bool Initialize();
	bool Broadcast();
	bool IsRunning();
	bool Release();

	RECT GetClientWindowRect() const;
	void SetWindowHandle(HWND hwnd);

	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnDestroy();

protected:
	HWND m_hwnd = nullptr;
	bool m_is_running;
};

