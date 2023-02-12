#include <cmath>
#include <cassert>
#include <Windows.h>
#include "App.h"
#include "Input.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Graphics.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"

struct Vertex
{
	Vector3 position;
	Vector3 color;
	Vector3 color1;
};

__declspec(align(16))
struct Constant
{
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 projection;
	unsigned int time = 0;
};

void App::OnCreate()
{
	Window::OnCreate();

	Input::GetInstance()->AddListener(this);

	Graphics::GetInstance()->Initialize();
	m_swap_chain = Graphics::GetInstance()->CreateSwapChain();
	assert(m_swap_chain);

	RECT rect = GetClientWindowRect();
	m_swap_chain->Initialize(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);
	assert(m_swap_chain);

	Vertex vertices[] =
	{
		// Front face
		{ Vector3(-0.5f, -0.5f, -0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3(-0.5f, 0.5f, -0.5f), Vector3(1.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f) },
		{ Vector3(0.5f, 0.5f, -0.5f), Vector3(1.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f) },
		{ Vector3(0.5f, -0.5f, -0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f) },

		// Back face
		{ Vector3(0.5f, -0.5f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector3(0.5f, 0.5f, 0.5f), Vector3(0.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 1.0f) },
		{ Vector3(-0.5f, 0.5f, 0.5f), Vector3(0.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 1.0f) },
		{ Vector3(-0.5f, -0.5f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) }
	};
	UINT vertex_count = ARRAYSIZE(vertices);

	m_vertex_buffer = Graphics::GetInstance()->CreateVertexBuffer();
	assert(m_vertex_buffer);

	unsigned int indices[] =
	{
		// Bront side
		0, 1, 2,	// First triangle
		2, 3, 0,	// Second triangle

		// Back side
		4, 5, 6,
		6, 7, 4,

		// Top side
		1, 6, 5,
		5, 2, 1,

		// Bottom size
		7, 0, 3,
		3, 4, 7,

		// Right size
		3, 2, 5,
		5, 4, 3,

		// Left side
		7, 6, 1,
		1, 0, 7
	};
	UINT index_count = ARRAYSIZE(indices);

	m_index_buffer = Graphics::GetInstance()->CreateIndexBuffer();
	assert(m_index_buffer);
	m_index_buffer->Load(indices, index_count);

	void* shader_byte_code = nullptr;
	size_t shader_byte_size = 0;
	Graphics::GetInstance()->CompileVertexShader(L"VertexShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_vertex_shader = Graphics::GetInstance()->CreateVertexShader(shader_byte_code, shader_byte_size);
	assert(m_vertex_shader);
	m_vertex_buffer->Load(vertices, sizeof(Vertex), vertex_count, shader_byte_code, shader_byte_size);
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

	Input::GetInstance()->Update();

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

	// 인덱스 버퍼 설정
	Graphics::GetInstance()->GetDeviceContext()->SetIndexBuffer(m_index_buffer);

	// 삼각형 그리기
	Graphics::GetInstance()->GetDeviceContext()->DrawIndexedTriangleList(m_index_buffer->GetIndexCount(), 0, 0);
	m_swap_chain->Present(true);

	// 프레임 설정
	m_old_delta = m_new_delta;
	m_new_delta = static_cast<float>(::GetTickCount64());
	m_delta_time = m_old_delta ? (m_new_delta - m_old_delta) / 1000.0f : 0.0f;
}

void App::OnFocus()
{
	Input::GetInstance()->AddListener(this);
}

void App::OnKillFocus()
{
	Input::GetInstance()->RemoveListener(this);
}

void App::OnDestroy()
{
	Window::OnDestroy();

	assert(m_pixel_shader);
	m_pixel_shader->Release();
	assert(m_vertex_shader);
	m_vertex_shader->Release();
	assert(m_index_buffer);
	m_index_buffer->Release();
	assert(m_vertex_buffer);
	m_vertex_buffer->Release();
	assert(m_constant_buffer);
	m_constant_buffer->Release();
	assert(m_swap_chain);
	m_swap_chain->Release();

	Graphics::GetInstance()->Release();
}

void App::OnKeyUp(int key)
{
}

void App::OnKeyDown(int key)
{
	if (key == 'W')
	{
		m_rotation_x += 3.14f * m_delta_time;
	}
	else if (key == 'S')
	{
		m_rotation_x -= 3.14f * m_delta_time;
	}
	else if (key == 'A')
	{
		m_rotation_y += 3.14f * m_delta_time;
	}
	else if (key == 'D')
	{
		m_rotation_y -= 3.14f * m_delta_time;
	}
}

void App::OnMouseMove(const Point& point)
{
	m_rotation_x -= point.GetY() * m_delta_time;
	m_rotation_y -= point.GetX() * m_delta_time;
}

void App::OnLeftButtonUp(const Point& point)
{
	m_scale_cube = 1.0f;
}

void App::OnLeftButtonDown(const Point& point)
{
	m_scale_cube = 0.5f;
}

void App::OnRightButtonUp(const Point& point)
{
	m_scale_cube = 1.0f;
}

void App::OnRightButtonDown(const Point& point)
{
	m_scale_cube = 2.0f;
}

void App::UpdateQuadPosition()
{
	m_delta_position += m_delta_time / 10.0f;
	if (m_delta_position >= 1.0f)
		m_delta_position = 0.0f;

	m_delta_scale += m_delta_time / 0.55f;

	Matrix4x4 temp;
	//temp.SetTranslation(Vector3::LinearInterpolation(Vector3(-1.5f, -1.5f, 0.0f), Vector3(1.5f, 1.5f, 0.0f), m_delta_position));

	Constant constant;
	constant.time = static_cast<unsigned int>(::GetTickCount64());
	constant.world.SetScale(Vector3(m_scale_cube, m_scale_cube, m_scale_cube));
	//constant.world.SetTranslation(Vector3::LinearInterpolation(Vector3(-2.0f, -2.0f, 0.0f), Vector3(2.0f, 2.0f, 0.0f), m_delta_position));
	//constant.world.SetScale(Vector3::LinearInterpolation(Vector3(0.5f, 0.5f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), (std::sin(m_delta_scale) + 1.0f) / 2.0f));

	temp.SetIdentity();
	temp.SetRotationX(m_rotation_x);
	constant.world *= temp;

	temp.SetIdentity();
	temp.SetRotationY(m_rotation_y);
	constant.world *= temp;

	temp.SetIdentity();
	temp.SetRotationZ(m_rotation_z);
	constant.world *= temp;

	constant.view.SetIdentity();
	constant.projection.SetOrthographicProjection
	(
		(GetClientWindowRect().right - GetClientWindowRect().left) / 300.0f,
		(GetClientWindowRect().bottom - GetClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);
	m_constant_buffer->Update(Graphics::GetInstance()->GetDeviceContext(), &constant);
}
