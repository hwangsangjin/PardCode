#include "Graphics.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include <cassert>

bool Graphics::Initialize()
{
    D3D_DRIVER_TYPE driver_types[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };
    UINT num_driver_types = ARRAYSIZE(driver_types);

    D3D_FEATURE_LEVEL feature_levels[] =
    {
        D3D_FEATURE_LEVEL_11_0
    };
    UINT num_feature_levels = ARRAYSIZE(feature_levels);

    ID3D11DeviceContext* immediate_context = nullptr;
    for (UINT driver_type_index = 0; driver_type_index < num_driver_types; driver_type_index++)
    {
        HRESULT hr = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels, num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &immediate_context);
        if (SUCCEEDED(hr))
        {
            break;
        }

        ++driver_type_index;
    }

    assert(m_d3d_device);
    assert(m_feature_level);
    assert(immediate_context);

    m_device_context = new DeviceContext(immediate_context);

    m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
    m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
    m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);


    return true;
}

bool Graphics::Release()
{
    m_dxgi_factory->Release();
    m_dxgi_adapter->Release();
    m_dxgi_device->Release();
    m_device_context->Release();
    m_d3d_device->Release();

    return true;
}

Graphics* Graphics::GetInstance()
{
    static Graphics instance;
    return &instance;
}

ID3D11Device* Graphics::GetD3DDevice() const
{
    return m_d3d_device;
}

IDXGIFactory* Graphics::GetDXGIFactory() const
{
    return m_dxgi_factory;
}

SwapChain* Graphics::CreateSwapChain()
{
    return new SwapChain();
}

DeviceContext* Graphics::GetDeviceContext() const
{
    return m_device_context;
}
