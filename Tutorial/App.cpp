#include <cassert>
#include "App.h"
#include "Graphics.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

struct Vec3
{
	float x, y, z;
};

struct Vertex
{
	Vec3 position;
	Vec3 color;
};

void App::OnCreate()
{
	Window::OnCreate();

	Graphics::GetInstance()->Initialize();
	m_swap_chain = Graphics::GetInstance()->CreateSwapChain();
	assert(m_swap_chain);

	RECT rect = GetClientWindowRect();
	m_swap_chain->Initialize(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	Vertex vertices[] =
	{
		{ -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f },
		{ -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f },
		{ 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f }
	};
	UINT vertex_size = ARRAYSIZE(vertices);

	m_vertex_buffer = Graphics::GetInstance()->CreateVertexBuffer();
	assert(m_vertex_buffer);

	void* shader_byte_code = nullptr;
	size_t shader_byte_size = 0;
	Graphics::GetInstance()->CompileVertexShader(L"VertexShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	m_vertex_shader = Graphics::GetInstance()->CreateVertexShader(shader_byte_code, shader_byte_size);
	m_vertex_buffer->Load(vertices, sizeof(Vertex), vertex_size, shader_byte_code, shader_byte_size);
	Graphics::GetInstance()->ReleaseCompiledShader();

	Graphics::GetInstance()->CompilePixelShader(L"PixelShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	m_pixel_shader = Graphics::GetInstance()->CreatePixelShader(shader_byte_code, shader_byte_size);
	Graphics::GetInstance()->ReleaseCompiledShader();
}

void App::OnUpdate()
{
	Window::OnUpdate();

	// 렌더 타겟 지우기
	Graphics::GetInstance()->GetDeviceContext()->ClearRenderTargetColor(m_swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);

	// 뷰포트 설정
	RECT rect = GetClientWindowRect();
	Graphics::GetInstance()->GetDeviceContext()->SetViewportSize(static_cast<UINT>(rect.right - rect.left), static_cast<UINT>(rect.bottom - rect.top));

	// 셰이더 설정
	Graphics::GetInstance()->GetDeviceContext()->SetVertexShader(m_vertex_shader);
	Graphics::GetInstance()->GetDeviceContext()->SetPixelShader(m_pixel_shader);

	// 정점 버퍼 설정
	Graphics::GetInstance()->GetDeviceContext()->SetVertexBuffer(m_vertex_buffer);

	// 삼각형 그리기
	Graphics::GetInstance()->GetDeviceContext()->DrawTriangleStrip(m_vertex_buffer->GetVertexSize(), 0);
	m_swap_chain->Present(false);
}

void App::OnDestroy()
{
	Window::OnDestroy();
	m_pixel_shader->Release();
	m_vertex_shader->Release();
	m_vertex_buffer->Release();
	m_swap_chain->Release();
	Graphics::GetInstance()->Release();
}
