#pragma once

#include "Window.h"

class SwapChain;
class VertexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;

class App : public Window
{
public:
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;

	void UpdateQuadPosition();

private:
	SwapChain* m_swap_chain = nullptr;
	VertexBuffer* m_vertex_buffer = nullptr;
	VertexShader* m_vertex_shader = nullptr;
	PixelShader* m_pixel_shader = nullptr;
	ConstantBuffer* m_constant_buffer = nullptr;

	float m_old_delta = 0.0f;
	float m_new_delta = 0.0f;
	float m_delta_time = 0.0f;
	float m_delta_position = 0.0f;
	float m_delta_scale = 0.0f;
};

