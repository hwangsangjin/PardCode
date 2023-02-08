#include "SwapChain.h"
#include "Graphics.h"
#include <cassert>

bool SwapChain::Initialize(HWND hwnd, UINT width, UINT height)
{
    // 스왑 체인 구조체
    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.BufferCount = 1;
    desc.BufferDesc.Width = width;
    desc.BufferDesc.Height = height;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = hwnd;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Windowed = TRUE;

    // 스왑 체인 생성
    ID3D11Device* device = Graphics::GetInstance()->GetD3DDevice();
    Graphics::GetInstance()->GetDXGIFactory()->CreateSwapChain(device, &desc, &m_dxgi_swap_chain);
    assert(&m_dxgi_swap_chain);

    return true;
}

bool SwapChain::Release()
{
    m_dxgi_swap_chain->Release();
    delete this;

    return true;
}
