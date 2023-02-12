#pragma once

#include "Window.h"
#include "Matrix4x4.h"
#include "Engine.h"

class SwapChain;
class VertexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;
class IndexBuffer;

class App : public Window
{
public:
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnFocus() override;
	virtual void OnKillFocus() override;
	virtual void OnDestroy() override;

	virtual void OnKeyUp(int key) override;
	virtual void OnKeyDown(int key) override;

	virtual void OnMouseMove(const Point& point) override;

	virtual void OnLeftButtonUp(const Point& point) override;
	virtual void OnLeftButtonDown(const Point& point) override;
	virtual void OnRightButtonUp(const Point& point) override;
	virtual void OnRightButtonDown(const Point& point) override;

	void Update();

private:
	SwapChainPtr m_swap_chain = nullptr;
	VertexBufferPtr m_vertex_buffer = nullptr;
	VertexShaderPtr m_vertex_shader = nullptr;
	PixelShaderPtr m_pixel_shader = nullptr;
	ConstantBufferPtr m_constant_buffer = nullptr;
	IndexBufferPtr m_index_buffer = nullptr;

	float m_old_delta = 0.0f;
	float m_new_delta = 0.0f;
	float m_delta_time = 0.0f;
	float m_delta_position = 0.0f;
	float m_delta_scale = 0.0f;
	float m_delta_rotation = 0.0f;
	float m_rotation_x = 0.0f;
	float m_rotation_y = 0.0f;
	float m_rotation_z = 0.0f;
	float m_scale_cube = 1.0f;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	Matrix4x4 m_world_camera = {};
};

