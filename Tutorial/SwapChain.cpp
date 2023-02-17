#include <cassert>
#include <exception>
#include "SwapChain.h"
#include "Graphics.h"

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, Graphics* graphics)
    : m_graphics(graphics)
{
    // ���� ü�� ����ü
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
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    desc.Windowed = TRUE;

    // ���� ü�� ����
    ID3D11Device* device = m_graphics->GetD3DDevice();
    HRESULT hr = m_graphics->GetDXGIFactory()->CreateSwapChain(device, &desc, &m_dxgi_swap_chain);
    if (FAILED(hr))
    {
        assert(&m_dxgi_swap_chain);
        throw std::exception("SwapChain not created successfully");
    }

    LoadBuffers(width, height);
}

SwapChain::~SwapChain()
{
    if (m_render_target_view)
    {
        m_render_target_view->Release();
        m_render_target_view = nullptr;
    }

    if (m_depth_stencil_view)
    {
        m_depth_stencil_view->Release();
        m_depth_stencil_view = nullptr;
    }

    if (m_dxgi_swap_chain)
    {
        m_dxgi_swap_chain->Release();
        m_dxgi_swap_chain = nullptr;
    }
}

void SwapChain::SetFullScreen(bool fullscreen, unsigned int width, unsigned int height)
{
    Resize(width, height);
    m_dxgi_swap_chain->SetFullscreenState(fullscreen, nullptr);
}

void SwapChain::Resize(unsigned int width, unsigned int height)
{
    if (m_render_target_view)
    {
        m_render_target_view->Release();
        m_render_target_view = nullptr;
    }

    if (m_depth_stencil_view)
    {
        m_depth_stencil_view->Release();
        m_depth_stencil_view = nullptr;
    }

    m_dxgi_swap_chain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0); 
    LoadBuffers(width, height);
}

void SwapChain::Present(bool vsync)
{
    m_dxgi_swap_chain->Present(vsync, NULL);
}

ID3D11RenderTargetView* SwapChain::GetRenderTargetView() const
{
    return m_render_target_view;
}

ID3D11DepthStencilView* SwapChain::GetDepthStencilView() const
{
    return m_depth_stencil_view;
}

void SwapChain::LoadBuffers(unsigned int width, unsigned int height)
{
    // �ĸ� ���� ������ �����ͼ� ���� Ÿ�� �並 ����
    ID3D11Device* device = m_graphics->GetD3DDevice();
    ID3D11Texture2D* buffer = NULL;

    HRESULT hr = m_dxgi_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
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

    D3D11_TEXTURE2D_DESC texture_desc = {};
    texture_desc.Width = width;
    texture_desc.Height = height;
    texture_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    texture_desc.Usage = D3D11_USAGE_DEFAULT;
    texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    texture_desc.MipLevels = 1;
    texture_desc.SampleDesc.Count = 1;
    texture_desc.SampleDesc.Quality = 0;
    texture_desc.MiscFlags = 0;
    texture_desc.ArraySize = 1;
    texture_desc.CPUAccessFlags = 0;

    hr = device->CreateTexture2D(&texture_desc, nullptr, &buffer);
    if (FAILED(hr))
    {
        assert(buffer);
        throw std::exception("Texture2D not created successfully");
    }

    hr = device->CreateDepthStencilView(buffer, NULL, &m_depth_stencil_view);
    buffer->Release();

    if (FAILED(hr))
    {
        assert(m_depth_stencil_view);
        throw std::exception("DepthStencilView not created successfully");
    }
}
