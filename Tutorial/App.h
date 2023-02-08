#pragma once

#include "Window.h"

class SwapChain;

class App : public Window
{
public:
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;

private:
	SwapChain* m_swap_chain;
};

