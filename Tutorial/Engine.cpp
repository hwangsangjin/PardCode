#include <cassert>
#include <exception>
#include "Engine.h"
#include "Graphics.h"

Engine* Engine::m_engine = nullptr;

void Engine::Create()
{
    if (Engine::m_engine)
        throw std::exception("Engine already created");

    Engine::m_engine = new Engine();
}

void Engine::Release()
{
    if (!Engine::m_engine)
        return;

    delete Engine::m_engine;
    Engine::m_engine = nullptr;
}

Engine* Engine::GetInstance()
{
    return m_engine;
}

Graphics* Engine::GetGraphics()
{
    return m_graphics;
}

Engine::Engine()
{
    try
    {
        m_graphics = new Graphics();
    }
    catch (...) { throw std::exception("Engine not created successfully"); }
}

Engine::~Engine()
{
    if (m_graphics)
    {
        delete m_graphics;
        m_graphics = nullptr;
    }
}
