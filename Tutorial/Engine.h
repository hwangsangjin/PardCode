#pragma once

#include "Prerequisites.h"
#include "Graphics.h"

class Engine
{
public:
	static void Create();
	static void Release();
	static Engine* GetInstance();
	Graphics* GetGraphics();

private:
	Engine();
	~Engine();

	static Engine* m_engine;
	Graphics* m_graphics = nullptr;
};

