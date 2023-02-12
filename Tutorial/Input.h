#pragma once

#include <unordered_set>
#include "Point.h"

class App;

class Input
{
public:
	static Input* GetInstance();

	void Update();
	void AddListener(App* listener);
	void RemoveListener(App* listener);

private:
	std::unordered_set<App*> m_set_listeners;
	unsigned char m_key_states[256] = {};
	unsigned char m_old_key_states[256] = {};
	Point m_old_mouse_position = {};
	bool m_first_time = true;
};

