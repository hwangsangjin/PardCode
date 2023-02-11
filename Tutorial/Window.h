#pragma once

#include <Windows.h>

class Window 
{
public:
	bool Initialize();
	bool IsRunning() const;
	void Broadcast();
	void Release();

	RECT GetClientWindowRect() const;
	void SetWindowHandle(HWND hwnd);

	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnDestroy();

	virtual void OnKeyUp(int key);
	virtual void OnKeyDown(int key);

protected:
	HWND m_hwnd = nullptr;
	bool m_is_running = false;
};

