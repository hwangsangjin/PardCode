#include <cassert>
#include <exception>
#include "Engine.h"
#include "TextureManager.h"

Engine* Engine::m_engine = nullptr;

void Engine::Create()
{
    assert(!m_engine);
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

TextureManager* Engine::GetTextureManager()
{
    return m_texture_manager;
}

Engine::Engine()
{
    try
    {
        m_graphics = new Graphics();
    }
    catch (...)
    {
        assert(m_graphics);
        throw std::exception("Graphics not created successfully");
    }

    try
    {
        m_texture_manager = new TextureManager();
    }
    catch (...)
    {
        assert(m_texture_manager);
        throw std::exception("TextureManager not created successfully");
    }
}

Engine::~Engine()
{
    if (m_graphics)
    {
        delete m_graphics;
        m_graphics = nullptr;
    }

    if (m_texture_manager)
    {
        delete m_texture_manager;
        m_texture_manager = nullptr;
    }
}
