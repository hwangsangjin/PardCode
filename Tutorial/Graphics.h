#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	SwapChainPtr CreateSwapChain(HWND hwnd, UINT width, UINT height);
	ConstantBufferPtr CreateConstantBuffer(void* buffer, UINT buffer_size);
	IndexBufferPtr CreateIndexBuffer(void* indices, UINT index_count);
	VertexBufferPtr CreateVertexBuffer(void* vertices, UINT vertex_size, UINT vertex_count, void* shader_byte_code, size_t shader_byte_size);
	VertexShaderPtr CreateVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShaderPtr CreatePixelShader(const void* shader_byte_code, size_t byte_code_size);
	void CompileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	void CompilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	void ReleaseCompiledShader();

	ID3D11Device* GetD3DDevice() const;
	IDXGIFactory* GetDXGIFactory() const;
	DeviceContextPtr GetDeviceContext() const;

	void SetRasterizerState(bool cull_front);

private:
	void InitializeRasterizerState();

	D3D_FEATURE_LEVEL m_feature_level = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device* m_d3d_device = nullptr;
	DeviceContextPtr m_device_context = nullptr;
	ID3D11DeviceContext* m_immediate_context = nullptr;
	IDXGIDevice* m_dxgi_device = nullptr;
	IDXGIAdapter* m_dxgi_adapter = nullptr;
	IDXGIFactory* m_dxgi_factory = nullptr;
	ID3DBlob* m_blob = nullptr;
	ID3D11VertexShader* m_vertex_shader = nullptr;
	ID3D11PixelShader* m_pixel_shader = nullptr;
	ID3D11RasterizerState* m_cull_front_state = nullptr;
	ID3D11RasterizerState* m_cull_back_state = nullptr;
};

