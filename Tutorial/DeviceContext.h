#pragma once

#include "Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context, Graphics* graphics);
	~DeviceContext();

	ID3D11DeviceContext* GetDeviceContext();
	void SetViewportSize(UINT width, UINT height);
	void SetVertexBuffer(VertexBufferPtr vertex_buffer);
	void SetVertexShader(VertexShaderPtr vertex_shader);
	void SetPixelShader(PixelShaderPtr pixel_shader);
	void SetConstantBuffer(ConstantBufferPtr constant_buffer, VertexShaderPtr vertex_shader);
	void SetConstantBuffer(ConstantBufferPtr constant_buffer, PixelShaderPtr pixel_shader);
	void SetIndexBuffer(IndexBufferPtr index_buffer);
	void DrawTriangleList(UINT vertex_count, UINT start_vertex_location);
	void DrawIndexedTriangleList(UINT index_count, UINT start_index_location, UINT base_vertex_location);
	void DrawTriangleStrip(UINT vertex_count, UINT start_vertex_location);
	void ClearRenderTargetColor(SwapChainPtr swap_chain, float red, float green, float blue, float alpha);

private:
	ID3D11DeviceContext* m_device_context = nullptr;
	Graphics* m_graphics = nullptr;
};

