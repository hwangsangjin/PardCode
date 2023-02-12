#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, Graphics* graphics);
	~SwapChain();

	void Present(bool vsync);

	ID3D11RenderTargetView* GetRenderTargetView() const;

private:
	IDXGISwapChain* m_dxgi_swap_chain = nullptr;
	ID3D11RenderTargetView* m_render_target_view = nullptr;
	ID3D11DepthStencilView* m_depth_stencil_view = nullptr;
	Graphics* m_graphics = nullptr;
};

