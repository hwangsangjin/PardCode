#include <cassert>
#include <Windows.h>
#include "App.h"
#include "Graphics.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"

struct Vec3
{
	float x, y, z;
};

struct Vertex
{
	Vec3 position;
	Vec3 position1;
	Vec3 color;
	Vec3 color1;
};

__declspec(align(16))
struct Constant
{
	unsigned int time;
};

void App::OnCreate()
{
	Window::OnCreate();

	Graphics::GetInstance()->Initialize();
	m_swap_chain = Graphics::GetInstance()->CreateSwapChain();
	assert(m_swap_chain);

	RECT rect = GetClientWindowRect();
	m_swap_chain->Initialize(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);
	assert(m_swap_chain);

	Vertex vertices[] =
	{
		{ -0.5f, -0.5f, 0.0f, -0.32f, -0.11f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
		{ -0.5f, 0.5f, 0.0f, -0.11f, 0.78f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 0.75f, -0.73f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ 0.5f, 0.5f, 0.0f, 0.88f, 0.77f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f }
	};
	UINT vertex_size = ARRAYSIZE(vertices);

	m_vertex_buffer = Graphics::GetInstance()->CreateVertexBuffer();
	assert(m_vertex_buffer);

	void* shader_byte_code = nullptr;
	size_t shader_byte_size = 0;
	Graphics::GetInstance()->CompileVertexShader(L"VertexShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_vertex_shader = Graphics::GetInstance()->CreateVertexShader(shader_byte_code, shader_byte_size);
	assert(m_vertex_shader);
	m_vertex_buffer->Load(vertices, sizeof(Vertex), vertex_size, shader_byte_code, shader_byte_size);
	assert(m_vertex_buffer);
	Graphics::GetInstance()->ReleaseCompiledShader();

	Graphics::GetInstance()->CompilePixelShader(L"PixelShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_pixel_shader = Graphics::GetInstance()->CreatePixelShader(shader_byte_code, shader_byte_size);
	assert(m_pixel_shader);
	Graphics::GetInstance()->ReleaseCompiledShader();

	Constant constant;
	constant.time = 0;
	m_constant_buffer = Graphics::GetInstance()->CreateConstantBuffer();
	assert(m_constant_buffer);
	m_constant_buffer->Load(&constant, sizeof(Constant));
	assert(&constant);
}

void App::OnUpdate()
{
	Window::OnUpdate();

	// ���� Ÿ�� �����
	Graphics::GetInstance()->GetDeviceContext()->ClearRenderTargetColor(m_swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);

	// ����Ʈ ����
	RECT rect = GetClientWindowRect();
	Graphics::GetInstance()->GetDeviceContext()->SetViewportSize(static_cast<UINT>(rect.right - rect.left), static_cast<UINT>(rect.bottom - rect.top));

	// ��� ���� ����
	Constant constant;
	constant.time = static_cast<unsigned int>(::GetTickCount64());
	m_constant_buffer->Update(Graphics::GetInstance()->GetDeviceContext(), &constant);
	Graphics::GetInstance()->GetDeviceContext()->SetConstantBuffer(m_constant_buffer, m_vertex_shader);
	Graphics::GetInstance()->GetDeviceContext()->SetConstantBuffer(m_constant_buffer, m_pixel_shader);

	// ���̴� ����
	Graphics::GetInstance()->GetDeviceContext()->SetVertexShader(m_vertex_shader);
	Graphics::GetInstance()->GetDeviceContext()->SetPixelShader(m_pixel_shader);

	// ���� ���� ����
	Graphics::GetInstance()->GetDeviceContext()->SetVertexBuffer(m_vertex_buffer);

	// �ﰢ�� �׸���
	Graphics::GetInstance()->GetDeviceContext()->DrawTriangleStrip(m_vertex_buffer->GetVertexSize(), 0);
	m_swap_chain->Present(false);
}

void App::OnDestroy()
{
	Window::OnDestroy();
	
	assert(m_pixel_shader);
	m_pixel_shader->Release();
	assert(m_vertex_shader);
	m_vertex_shader->Release();
	assert(m_vertex_buffer);
	m_vertex_buffer->Release();
	assert(m_swap_chain);
	m_swap_chain->Release();

	Graphics::GetInstance()->Release();
}
