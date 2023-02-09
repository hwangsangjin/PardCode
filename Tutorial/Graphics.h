#pragma once

#include <d3d11.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;

class Graphics
{
public:
	void Initialize();
	void Release();

	static Graphics* GetInstance();
	ID3D11Device* GetD3DDevice() const;
	IDXGIFactory* GetDXGIFactory() const;
	DeviceContext* GetDeviceContext() const;
	SwapChain* CreateSwapChain();
	VertexBuffer* CreateVertexBuffer();
	void CreateShaders();
	void SetShaders();
	void GetShaderBufferAndSize(void** shader_code_byte, UINT* size);

private:
	ID3D11Device* m_d3d_device = nullptr;
	D3D_FEATURE_LEVEL m_feature_level;
	DeviceContext* m_device_context = nullptr;
	ID3D11DeviceContext* m_immediate_context = nullptr;
	IDXGIDevice* m_dxgi_device = nullptr;
	IDXGIAdapter* m_dxgi_adapter = nullptr;
	IDXGIFactory* m_dxgi_factory = nullptr;
	ID3DBlob* m_vertex_shader_blob = nullptr;
	ID3DBlob* m_pixel_shader_blob = nullptr;
	ID3D11VertexShader* m_vertex_shader = nullptr;
	ID3D11PixelShader* m_pixel_shader = nullptr;
};

