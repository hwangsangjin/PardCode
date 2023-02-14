#include <cmath>
#include <cassert>
#include <Windows.h>
#include "App.h"
#include "Input.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"

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
	Vector4 light_direction;
	Vector4 camera_position;
};

void App::OnCreate()
{
	Window::OnCreate();

	Input::GetInstance()->AddListener(this);
	Input::GetInstance()->ShowCursor(false);

	m_mesh = Engine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\suzanne.obj");
	m_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\brick.png");

	m_skybox_mesh = Engine::GetInstance()->GetMeshManager()->CreateMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	m_skybox_texture = Engine::GetInstance()->GetTextureManager()->CreateTextureFromFile(L"Assets\\Textures\\sky.jpg");

	RECT rect = GetClientWindowRect();
	m_swap_chain = Engine::GetInstance()->GetGraphics()->CreateSwapChain(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);
	assert(m_swap_chain);

	m_world.SetTranslation(Vector3(0.0f, 0.0f, -1.0f));

	// 정점 셰이더 생성
	void* shader_byte_code = nullptr;
	size_t shader_byte_size = 0;
	Engine::GetInstance()->GetGraphics()->CompileVertexShader(L"VertexShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	m_vertex_shader = Engine::GetInstance()->GetGraphics()->CreateVertexShader(shader_byte_code, shader_byte_size);
	assert(m_vertex_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	// 픽셀 셰이더 생성
	Engine::GetInstance()->GetGraphics()->CompilePixelShader(L"PixelShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	m_pixel_shader = Engine::GetInstance()->GetGraphics()->CreatePixelShader(shader_byte_code, shader_byte_size);
	assert(m_pixel_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	Engine::GetInstance()->GetGraphics()->CompilePixelShader(L"SkyBoxShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	m_skybox_pixel_shader = Engine::GetInstance()->GetGraphics()->CreatePixelShader(shader_byte_code, shader_byte_size);
	assert(m_skybox_pixel_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	// 상수 버퍼 생성
	Constant constant;
	m_constant_buffer = Engine::GetInstance()->GetGraphics()->CreateConstantBuffer(&constant, sizeof(Constant));
	assert(m_constant_buffer);
	m_skybox_constant_buffer = Engine::GetInstance()->GetGraphics()->CreateConstantBuffer(&constant, sizeof(Constant));
	assert(m_skybox_constant_buffer);
}

void App::OnUpdate()
{
	Window::OnUpdate();

	Input::GetInstance()->Update();

	// 프레임 설정
	m_old_delta = m_new_delta;
	m_new_delta = static_cast<float>(::GetTickCount64());
	m_delta_time = m_old_delta ? (m_new_delta - m_old_delta) / 1000.0f : 0.0f;

	// 렌더 타겟 지우기
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->ClearRenderTargetColor(m_swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);

	// 뷰포트 설정
	RECT rect = GetClientWindowRect();
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetViewportSize(static_cast<UINT>(rect.right - rect.left), static_cast<UINT>(rect.bottom - rect.top));

	// 변환 행렬 계산
	Update();

	// 렌더링
	Engine::GetInstance()->GetGraphics()->SetRasterizerState(false);
	Render(m_mesh, m_texture, m_vertex_shader, m_pixel_shader, m_constant_buffer);
	Engine::GetInstance()->GetGraphics()->SetRasterizerState(true);
	Render(m_skybox_mesh, m_skybox_texture, m_vertex_shader, m_skybox_pixel_shader, m_skybox_constant_buffer);

	// 페이지 플리핑
	m_swap_chain->Present(true);
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
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
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
	UpdateCamera();
	UpdateModel();
	UpdateSkyBox();
}

void App::UpdateCamera()
{
	Matrix4x4 world, temp;
	world.SetIdentity();

	temp.SetIdentity();
	temp.SetRotationX(m_rotation_x);
	world *= temp;

	temp.SetIdentity();
	temp.SetRotationY(m_rotation_y);
	world *= temp;

	Vector3 new_position = m_world.GetTranslation() + world.GetZDirection() * (m_forward * 0.05f);

	new_position = new_position + world.GetXDirection() * (m_rightward * 0.05f);

	world.SetTranslation(new_position);

	m_world = world;

	world.Inverse();

	m_view = world;

	int width = GetClientWindowRect().right - GetClientWindowRect().left;
	int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

	/*constant.projection.SetOrthographicProjection
	(
		(GetClientWindowRect().right - GetClientWindowRect().left) / 300.0f,
		(GetClientWindowRect().bottom - GetClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);*/

	m_projection.SetPerspectiveProjection(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
}

void App::UpdateModel()
{
	Matrix4x4 temp;
	Matrix4x4 light_rotation_matrix;
	light_rotation_matrix.SetIdentity();
	light_rotation_matrix.SetRotationY(m_light_rotation_y);
	m_light_rotation_y += 0.785f * m_delta_time;

	Constant constant;
	constant.world.SetIdentity();
	constant.view = m_view;
	constant.projection = m_projection;
	constant.camera_position = m_world.GetTranslation();
	constant.light_direction = light_rotation_matrix.GetZDirection();

	m_constant_buffer->Update(Engine::GetInstance()->GetGraphics()->GetDeviceContext(), &constant);
}

void App::UpdateSkyBox()
{
	Constant constant;
	constant.world.SetIdentity();
	constant.world.SetScale(Vector3(100.0f, 100.0f, 100.0f));
	constant.world.SetTranslation(m_world.GetTranslation());
	constant.view = m_view;
	constant.projection = m_projection;

	m_skybox_constant_buffer->Update(Engine::GetInstance()->GetGraphics()->GetDeviceContext(), &constant);
}

void App::Render(const MeshPtr& mesh, const TexturePtr& texture, const VertexShaderPtr& vertex_shader, const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& constant_buffer)
{
	// 상수 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetConstantBuffer(vertex_shader, constant_buffer);
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetConstantBuffer(pixel_shader, constant_buffer);

	// 셰이더 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetVertexShader(vertex_shader);
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetPixelShader(pixel_shader);
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetTexture(pixel_shader, texture);

	// 정점 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetVertexBuffer(mesh->GetVertexBuffer());

	// 인덱스 버퍼 설정
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->SetIndexBuffer(mesh->GetIndexBuffer());

	// 삼각형 그리기
	Engine::GetInstance()->GetGraphics()->GetDeviceContext()->DrawIndexedTriangleList(mesh->GetIndexBuffer()->GetIndexCount(), 0, 0);
}
