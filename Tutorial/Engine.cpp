#include "Engine.h"
#include "Graphics.h"
#include <cassert>

void Engine::Initialize()
{
    m_graphics = new Graphics();
    m_graphics->Initialize();
}

void Engine::Release()
{
    m_graphics->Release();
    delete m_graphics;
    m_graphics = nullptr;
}

Engine* Engine::GetInstance()
{
    static Engine instance;
    return &instance;
}

Graphics* Engine::GetGraphics()
{
    return m_graphics;
}
