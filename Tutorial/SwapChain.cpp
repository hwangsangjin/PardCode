#include <cassert>
#include <exception>
#include "SwapChain.h"
#include "Graphics.h"

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, Graphics* graphics)
    : m_graphics(graphics)
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
    ID3D11Device* device = m_graphics->GetD3DDevice();
    HRESULT hr = m_graphics->GetDXGIFactory()->CreateSwapChain(device, &desc, &m_dxgi_swap_chain);
    if (FAILED(hr))
    {
        assert(&m_dxgi_swap_chain);
        throw std::exception("SwapChain not created successfully");
    }

    // 후면 버퍼 색상을 가져와서 렌더 타겟 뷰를 생성
    ID3D11Texture2D* buffer = NULL;
    hr = m_dxgi_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
    if (FAILED(hr))
    {
        assert(buffer);
        throw std::exception("SwapChain get buffer not successfully");
    }

    hr = device->CreateRenderTargetView(buffer, nullptr, &m_render_target_view);
    buffer->Release();
    if (FAILED(hr))
    {
        assert(m_render_target_view);
        throw std::exception("RenderTargetView not created successfully");
    }
}

SwapChain::~SwapChain()
{
    if (m_dxgi_swap_chain)
    {
        m_dxgi_swap_chain->Release();
        m_dxgi_swap_chain = nullptr;
    }
}

void SwapChain::Present(bool vsync)
{
    m_dxgi_swap_chain->Present(vsync, NULL);
}

ID3D11RenderTargetView* SwapChain::GetRenderTargetView() const
{
    return m_render_target_view;
}
