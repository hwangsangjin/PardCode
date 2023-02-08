#include "App.h"
#include "Graphics.h"
#include "SwapChain.h"
#include "DeviceContext.h"

void App::OnCreate()
{
	Window::OnCreate();
	Graphics::GetInstance()->Initialize();
	m_swap_chain = Graphics::GetInstance()->CreateSwapChain();

	RECT rc = GetClientWindowRect();
	m_swap_chain->Initialize(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
}

void App::OnUpdate()
{
	Window::OnUpdate();
	Graphics::GetInstance()->GetDeviceContext()->ClearRenderTargetColor(m_swap_chain, 0.0f, 0.0f, 0.0f, 1.0f);
	m_swap_chain->Present(false);
}

void App::OnDestroy()
{
	Window::OnDestroy();
	m_swap_chain->Release();
	Graphics::GetInstance()->Release();
}
