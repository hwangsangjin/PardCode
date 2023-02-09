#pragma once

#include "Window.h"

class SwapChain;
class VertexBuffer;
class VertexShader;

class App : public Window
{
public:
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;

private:
	SwapChain* m_swap_chain = nullptr;
	VertexBuffer* m_vertex_buffer = nullptr;
	VertexShader* m_vertex_shader = nullptr;
};

