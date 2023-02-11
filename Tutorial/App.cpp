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
#include "Vector3.h"
#include "Matrix4x4.h"
#include <cmath>

struct Vertex
{
	Vector3 position;
	Vector3 position1;
	Vector3 color;
	Vector3 color1;
};

__declspec(align(16))
struct Constant
{
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 projection;
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
		{ Vector3(-0.5f, -0.5f, 0.0f), Vector3(-0.32f, -0.11f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector3(-0.5f, 0.5f, 0.0f), Vector3(-0.11f, 0.78f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 1.0f) },
		{ Vector3(0.5f, -0.5f, 0.0f), Vector3(0.75f, -0.73f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3(0.5f, 0.5f, 0.0f), Vector3(0.88f, 0.77f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f) }
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

	// 렌더 타겟 지우기
	Graphics::GetInstance()->GetDeviceContext()->ClearRenderTargetColor(m_swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);

	// 뷰포트 설정
	RECT rect = GetClientWindowRect();
	Graphics::GetInstance()->GetDeviceContext()->SetViewportSize(static_cast<UINT>(rect.right - rect.left), static_cast<UINT>(rect.bottom - rect.top));

	// 상수 버퍼 설정
	UpdateQuadPosition();
	Graphics::GetInstance()->GetDeviceContext()->SetConstantBuffer(m_constant_buffer, m_vertex_shader);
	Graphics::GetInstance()->GetDeviceContext()->SetConstantBuffer(m_constant_buffer, m_pixel_shader);

	// 셰이더 설정
	Graphics::GetInstance()->GetDeviceContext()->SetVertexShader(m_vertex_shader);
	Graphics::GetInstance()->GetDeviceContext()->SetPixelShader(m_pixel_shader);

	// 정점 버퍼 설정
	Graphics::GetInstance()->GetDeviceContext()->SetVertexBuffer(m_vertex_buffer);

	// 삼각형 그리기
	Graphics::GetInstance()->GetDeviceContext()->DrawTriangleStrip(m_vertex_buffer->GetVertexSize(), 0);
	m_swap_chain->Present(false);

	// 프레임 설정
	m_old_delta = m_new_delta;
	m_new_delta = static_cast<float>(::GetTickCount64());
	m_delta_time = m_old_delta ? (m_new_delta - m_old_delta) / 1000.0f : 0;
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

void App::UpdateQuadPosition()
{
	m_delta_position += m_delta_time / 10.0f;
	if (m_delta_position >= 1.0f)
		m_delta_position = 0.0f;

	m_delta_scale += m_delta_time / 1.0f;

	Constant constant;
	constant.time = static_cast<unsigned int>(::GetTickCount64());
	constant.world.SetTranslation(Vector3::LinearInterpolation(Vector3(-2.0f, -2.0f, 0.0f), Vector3(2.0f, 2.0f, 0.0f), m_delta_position));
	Matrix4x4 temp;
	temp.SetTranslation(Vector3::LinearInterpolation(Vector3(-1.5f, -1.5f, 0.0f), Vector3(1.5f, 1.5f, 0.0f), m_delta_position));
	constant.world.SetScale(Vector3::LinearInterpolation(Vector3(0.5f, 0.5f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), (std::sin(m_delta_scale) + 1.0f) / 2.0f));
	constant.world *= temp;
	constant.view.SetIdentity();
	constant.projection.SetOrthographicProjection
	(
		(GetClientWindowRect().right - GetClientWindowRect().left) / 400.0f,
		(GetClientWindowRect().bottom - GetClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);
	m_constant_buffer->Update(Graphics::GetInstance()->GetDeviceContext(), &constant);
}
