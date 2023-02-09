#include <cassert>
#include <d3dcompiler.h>
#include "Graphics.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

void Graphics::Initialize()
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

    m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
    m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
    m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

    m_device_context = new DeviceContext(m_immediate_context);
}

void Graphics::Release()
{
    m_pixel_shader->Release();
    m_pixel_shader_blob->Release();
    m_dxgi_factory->Release();
    m_dxgi_adapter->Release();
    m_dxgi_device->Release();
    m_immediate_context->Release();
    m_device_context->Release();
    m_d3d_device->Release();
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

DeviceContext* Graphics::GetDeviceContext() const
{
    return m_device_context;
}

SwapChain* Graphics::CreateSwapChain()
{
    return new SwapChain();
}

VertexBuffer* Graphics::CreateVertexBuffer()
{
    return new VertexBuffer();
}

VertexShader* Graphics::CreateVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
    VertexShader* vertex_shader = new VertexShader();
    vertex_shader->Initialize(shader_byte_code, byte_code_size);
    assert(vertex_shader);
    return vertex_shader;
}

void Graphics::CompileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
    ID3DBlob* error_blob = nullptr;
    ::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob);
    assert(m_blob);

    *shader_byte_code = m_blob->GetBufferPointer();
    *byte_code_size = m_blob->GetBufferSize();
}

void Graphics::ReleaseCompiledShader()
{
    m_blob->Release();
}

void Graphics::CreateShaders()
{
    ID3DBlob* error_blob = nullptr;
    ::D3DCompileFromFile(L"Shader.fx", nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &m_pixel_shader_blob, &error_blob);
    m_d3d_device->CreatePixelShader(m_pixel_shader_blob->GetBufferPointer(), m_pixel_shader_blob->GetBufferSize(), nullptr, &m_pixel_shader);
    assert(m_pixel_shader);
}

void Graphics::SetShaders()
{
    m_immediate_context->PSSetShader(m_pixel_shader, nullptr, 0);
}
