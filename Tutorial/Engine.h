#pragma once

#include "Prerequisites.h"

class Graphics;

class Engine
{
public:
	void Initialize();
	void Release();

	static Engine* GetInstance();
	Graphics* GetGraphics();

private:
	Graphics* m_graphics = nullptr;
};

