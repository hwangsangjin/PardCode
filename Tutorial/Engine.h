#pragma once

#include "Prerequisites.h"
#include "Graphics.h"
#include "TextureManager.h"

class Engine
{
public:
	static void Create();
	static void Release();
	static Engine* GetInstance();

	Graphics* GetGraphics();
	TextureManager* GetTextureManager();

private:
	Engine();
	~Engine();

	static Engine* m_engine;
	Graphics* m_graphics = nullptr;
	TextureManager* m_texture_manager = nullptr;
};

