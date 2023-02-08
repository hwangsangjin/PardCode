#pragma once

#include <d3d11.h>

class SwapChain
{
public:
	bool Initialize(HWND hwnd, UINT width, UINT height);
	bool Release();

private:
	IDXGISwapChain* m_dxgi_swap_chain = nullptr;
};

