#pragma once

#include <d3d11.h>

class DeviceContext;

class SwapChain
{
public:
	bool Initialize(HWND hwnd, UINT width, UINT height);
	bool Present(bool vsync);
	bool Release();

	ID3D11RenderTargetView* GetRenderTargetView() const;

private:
	IDXGISwapChain* m_dxgi_swap_chain = nullptr;
	ID3D11RenderTargetView* m_render_target_view = nullptr;
};

