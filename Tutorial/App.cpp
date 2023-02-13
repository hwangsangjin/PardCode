#include <cmath>
#include <cassert>
#include <Windows.h>
#include "App.h"
#include "Input.h"
#include "Vector2.h"
#include "Vector3.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

struct Vertex
{
	Vector3 position;
	Vector2 texcoord;
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
	Input::GetInstance()->ShowCursor(false);

	m_wood_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\wood.jpg");

	RECT rect = GetClientWindowRect();
	m_swap_chain = Engine::GetInstance()->GetGraphics()->CreateSwapChain(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);
	assert(m_swap_chain);

	m_world_camera.SetTranslation(Vector3(0.0f, 0.0f, -2.0f));

	Vector3 position[] =
	{
		// Front face
		{ Vector3(-0.5f, -0.5f, -0.5f) },
		{ Vector3(-0.5f, 0.5f, -0.5f) },
		{ Vector3(0.5f, 0.5f, -0.5f) },
		{ Vector3(0.5f, -0.5f, -0.5f) },

		// Back face
		{ Vector3(0.5f, -0.5f, 0.5f) },
		{ Vector3(0.5f, 0.5f, 0.5f) },
		{ Vector3(-0.5f, 0.5f, 0.5f) },
		{ Vector3(-0.5f, -0.5f, 0.5f) }
	};

	Vector2 texcoord[] =
	{
		{ Vector2(0.0f, 0.0f) },
		{ Vector2(0.0f, 1.0f) },
		{ Vector2(1.0f, 0.0f) },
		{ Vector2(1.0f, 1.0f) },
	};

	Vertex vertices[] =
	{
		{ position[0], texcoord[1] },
		{ position[1], texcoord[0] },
		{ position[2], texcoord[2] },
		{ position[3], texcoord[3] },

		{ position[4], texcoord[1] },
		{ position[5], texcoord[0] },
		{ position[6], texcoord[2] },
		{ position[7], texcoord[3] },

		{ position[1], texcoord[1] },
		{ position[6], texcoord[0] },
		{ position[5], texcoord[2] },
		{ position[2], texcoord[3] },

		{ position[7], texcoord[1] },
		{ position[0], texcoord[0] },
		{ position[3], texcoord[2] },
		{ position[4], texcoord[3] },

		{ position[3], texcoord[1] },
		{ position[2], texcoord[0] },
		{ position[5], texcoord[2] },
		{ position[4], texcoord[3] },

		{ position[7], texcoord[1] },
		{ position[6], texcoord[0] },
		{ position[1], texcoord[2] },
		{ position[0], texcoord[3] }
	};
	UINT vertex_count = ARRAYSIZE(vertices);

	unsigned int indices[] =
	{
		// Front side
		0, 1, 2,	// First triangle
		2, 3, 0,	// Second triangle

		// Back side
		4, 5, 6,
		6, 7, 4,

		// Top side
		8, 9, 10,
		10, 11, 8,

		// Bottom side
		12, 13, 14,
		14, 15, 12,

		// Right side
		16, 17, 18,
		18, 19, 16,

		// Left side
		20, 21, 22,
		22, 23, 20
	};
	UINT index_count = ARRAYSIZE(indices);

	void* shader_byte_code = nullptr;
	size_t shader_byte_size = 0;
	Engine::GetInstance()->GetGraphics()->CompileVertexShader(L"VertexShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);

	m_vertex_buffer = Engine::GetInstance()->GetGraphics()->CreateVertexBuffer(vertices, sizeof(Vertex), vertex_count, shader_byte_code, shader_byte_size);
	assert(m_vertex_buffer);

	m_vertex_shader = Engine::GetInstance()->GetGraphics()->CreateVertexShader(shader_byte_code, shader_byte_size);
	assert(m_vertex_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	Engine::GetInstance()->GetGraphics()->CompilePixelShader(L"PixelShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_pixel_shader = Engine::GetInstance()->GetGraphics()->CreatePixelShader(shader_byte_code, shader_byte_size);
	assert(m_pixel_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	m_index_buffer = Engine::GetInstance()->GetGraphics()->CreateIndexBuffer(indices, index_count);
	assert(m_index_buffer);

	Constant constant;
	constant.time = 0;
	m_constant_buffer = Engine::GetInstance()->GetGraphics()->CreateConstantBuffer(&constant, sizeof(Constant));
	assert(m_constant_buffer);
	assert(&constant);
}

void App::OnUpdate()
{
	Window::OnUpdate();

	Input::GetInstance()->Update();

	// 렌더 타겟 지우기
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->ClearRenderTargetColor(m_swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);

	// 뷰포트 설정
	RECT rect = GetClientWindowRect();
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetViewportSize(static_cast<UINT>(rect.right - rect.left), static_cast<UINT>(rect.bottom - rect.top));

	// 상수 버퍼 설정
	Update();
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetConstantBuffer(m_vertex_shader, m_constant_buffer);
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetConstantBuffer(m_pixel_shader, m_constant_buffer);

	// 셰이더 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetVertexShader(m_vertex_shader);
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetPixelShader(m_pixel_shader);
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetTexture(m_pixel_shader, m_wood_texture);

	// 정점 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetVertexBuffer(m_vertex_buffer);

	// 인덱스 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetIndexBuffer(m_index_buffer);

	// 삼각형 그리기
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->DrawIndexedTriangleList(m_index_buffer->GetIndexCount(), 0, 0);
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
}

void App::OnKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void App::OnKeyDown(int key)
{
	if (key == 'W')
	{
		//m_rotation_x += 3.14f * m_delta_time;
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		//m_rotation_x -= 3.14f * m_delta_time;
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		//m_rotation_y += 3.14f * m_delta_time;
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		//m_rotation_y -= 3.14f * m_delta_time;
		m_rightward = 1.0f;
	}
}

void App::OnMouseMove(const Point& point)
{
	int width = GetClientWindowRect().right - GetClientWindowRect().left;
	int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	m_rotation_x += (point.GetY() - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rotation_y += (point.GetX() - (width / 2.0f)) * m_delta_time * 0.1f;

	Input::GetInstance()->SetCursorPosition(Point(width / 2, height / 2));
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

void App::Update()
{
	Constant constant;
	constant.time = static_cast<unsigned int>(::GetTickCount64());

	m_delta_position += m_delta_time / 10.0f;
	if (m_delta_position >= 1.0f)
		m_delta_position = 0.0f;

	m_delta_scale += m_delta_time / 0.55f;

	Matrix4x4 temp;
	//temp.SetTranslation(Vector3::LinearInterpolation(Vector3(-1.5f, -1.5f, 0.0f), Vector3(1.5f, 1.5f, 0.0f), m_delta_position));

	//constant.world.SetScale(Vector3(m_scale_cube, m_scale_cube, m_scale_cube));
	//constant.world.SetTranslation(Vector3::LinearInterpolation(Vector3(-2.0f, -2.0f, 0.0f), Vector3(2.0f, 2.0f, 0.0f), m_delta_position));
	//constant.world.SetScale(Vector3::LinearInterpolation(Vector3(0.5f, 0.5f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), (std::sin(m_delta_scale) + 1.0f) / 2.0f));

	/*temp.SetIdentity();
	temp.SetRotationX(m_rotation_x);
	constant.world *= temp;

	temp.SetIdentity();
	temp.SetRotationY(m_rotation_y);
	constant.world *= temp;

	temp.SetIdentity();
	temp.SetRotationZ(m_rotation_z);
	constant.world *= temp;*/
	
	constant.world.SetIdentity();

	Matrix4x4 world_camera;
	world_camera.SetIdentity();

	temp.SetIdentity();
	temp.SetRotationX(m_rotation_x);
	world_camera *= temp;

	temp.SetIdentity();
	temp.SetRotationY(m_rotation_y);
	world_camera *= temp;

	Vector3 new_position = m_world_camera.GetTranslation() + world_camera.GetZDirection() * (m_forward * 0.3f);

	new_position = new_position + world_camera.GetXDirection() * (m_rightward * 0.3f);

	world_camera.SetTranslation(new_position);

	m_world_camera = world_camera;

	world_camera.Inverse();

	constant.view = world_camera;
	/*constant.projection.SetOrthographicProjection
	(
		(GetClientWindowRect().right - GetClientWindowRect().left) / 300.0f,
		(GetClientWindowRect().bottom - GetClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);*/

	int width = GetClientWindowRect().right - GetClientWindowRect().left;
	int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	constant.projection.SetPerspectiveProjection(1.57f, (float)(width / (float)height), 0.1f, 100.0f);

	m_constant_buffer->Update(Engine::GetInstance()->GetGraphics()->GetDeviceContext(), &constant);
}
