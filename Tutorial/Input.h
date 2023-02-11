#pragma once

#include <map>

class App;

class Input
{
public:
	static Input* GetInstance();

	void Update();
	void AddListener(App* listener);
	void RemoveListener(App* listener);

private:
	std::map<App*, App*> m_map_listeners;
	unsigned char m_key_states[256] = {};
	unsigned char m_old_key_states[256] = {};
};

