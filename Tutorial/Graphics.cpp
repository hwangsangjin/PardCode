#include "Graphics.h"
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

    for (UINT driver_type_index = 0; driver_type_index < num_driver_types; driver_type_index++)
    {
        HRESULT hr = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels, num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_immediate_context);
        if (SUCCEEDED(hr))
        {
            break;
        }

        ++driver_type_index;
    }

    assert(m_d3d_device);
    assert(m_feature_level);
    assert(m_immediate_context);

    return true;
}

bool Graphics::Release()
{
    m_immediate_context->Release();
    m_d3d_device->Release();

    return true;
}

Graphics* Graphics::GetInstance()
{
    static Graphics instance;
    return &instance;
}
