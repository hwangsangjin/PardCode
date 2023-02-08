#pragma once

#include "Window.h"

class App : public Window
{
public:
	App() = default;
	~App() = default;

	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;
};

