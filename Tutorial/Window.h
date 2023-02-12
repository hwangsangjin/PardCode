#pragma once

#include <Windows.h>

class Point;

class Window
{
public:
	Window();
	~Window();

	bool IsRunning();

	RECT GetClientWindowRect() const;

	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnFocus();
	virtual void OnKillFocus();
	virtual void OnDestroy();

	virtual void OnKeyUp(int key);
	virtual void OnKeyDown(int key);

	virtual void OnMouseMove(const Point& point);

	virtual void OnLeftButtonUp(const Point& point);
	virtual void OnLeftButtonDown(const Point& point);
	virtual void OnRightButtonUp(const Point& point);
	virtual void OnRightButtonDown(const Point& point);

protected:
	HWND m_hwnd = nullptr;
	bool m_is_running = false;
	bool m_is_initialize = false;

private:
	bool Broadcast();
};

