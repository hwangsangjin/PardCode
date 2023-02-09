#include "App.h"
#include "Graphics.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"

struct Vec3
{
	float x, y, z;
};

struct Vertex
{
	Vec3 position;
};

void App::OnCreate()
{
	Window::OnCreate();
	Graphics::GetInstance()->Initialize();
	m_swap_chain = Graphics::GetInstance()->CreateSwapChain();

	RECT rect = GetClientWindowRect();
	m_swap_chain->Initialize(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	Vertex vertices[] =
	{
		{ -0.5f, -0.5f, 0.0f },
		{ -0.5f, 0.5f, 0.0f },
		{ 0.5f, -0.5f, 0.0f },
		{ 0.5f, 0.5f, 0.0f }
	};
	UINT vertex_size = ARRAYSIZE(vertices);

	m_vertex_buffer = Graphics::GetInstance()->CreateVertexBuffer();
	Graphics::GetInstance()->CreateShaders();
	void* shader_byte_code = nullptr;
	UINT shader_byte_size = 0;
	Graphics::GetInstance()->GetShaderBufferAndSize(&shader_byte_code, &shader_byte_size);
	m_vertex_buffer->Load(vertices, sizeof(Vertex), vertex_size, shader_byte_code, shader_byte_size);
}

void App::OnUpdate()
{
	Window::OnUpdate();

	// 렌더 타겟 지우기
	Graphics::GetInstance()->GetDeviceContext()->ClearRenderTargetColor(m_swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);

	// 뷰포트 설정
	RECT rect = GetClientWindowRect();
	Graphics::GetInstance()->GetDeviceContext()->SetViewportSize(rect.right - rect.left, rect.bottom - rect.top);

	// 셰이더 설정
	Graphics::GetInstance()->SetShaders();

	// 정점 버퍼 설정
	Graphics::GetInstance()->GetDeviceContext()->SetVertexBuffer(m_vertex_buffer);

	// 삼각형 그리기
	Graphics::GetInstance()->GetDeviceContext()->DrawTriangleStrip(m_vertex_buffer->GetVertexSize(), 0);
	m_swap_chain->Present(false);
}

void App::OnDestroy()
{
	Window::OnDestroy();
	m_vertex_buffer->Release();
	m_swap_chain->Release();
	Graphics::GetInstance()->Release();
}
