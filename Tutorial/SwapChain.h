#pragma once

#include <d3d11.h>

class DeviceContext;

class SwapChain
{
public:
	void Initialize(HWND hwnd, UINT width, UINT height);
	void Present(bool vsync);
	void Release();

	ID3D11RenderTargetView* GetRenderTargetView() const;

private:
	IDXGISwapChain* m_dxgi_swap_chain = nullptr;
	ID3D11RenderTargetView* m_render_target_view = nullptr;
	ID3D11DepthStencilView* m_depth_stencil_view = nullptr;
};

