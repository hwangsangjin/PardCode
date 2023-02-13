#include <cassert>
#include <exception>
#include <d3d11.h>
#include "DeviceContext.h"
#include "Graphics.h"
#include "SwapChain.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, Graphics* graphics)
	: m_device_context(device_context)
	, m_graphics(graphics)
{
}

DeviceContext::~DeviceContext()
{
	if (m_device_context)
	{
		m_device_context->Release();
		m_device_context = nullptr;
	}
}

ID3D11DeviceContext* DeviceContext::GetDeviceContext()
{
	return m_device_context;
}

void DeviceContext::SetViewportSize(UINT width, UINT height)
{
	// ����Ʈ ����ü
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// ����Ʈ ����
	m_device_context->RSSetViewports(1, &viewport);
}

void DeviceContext::SetVertexBuffer(const VertexBufferPtr& vertex_buffer)
{
	UINT stride = vertex_buffer->GetVertexSize();
	UINT offset = 0;
	auto vertex_buffers = vertex_buffer->GetBuffer();
	assert(vertex_buffers);
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffers, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->GetInputLayout());
}

void DeviceContext::SetVertexShader(const VertexShaderPtr& vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->GetVertexShader(), nullptr, 0);
}

void DeviceContext::SetPixelShader(const PixelShaderPtr& pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->GetPixelShader(), nullptr, 0);
}

void DeviceContext::SetTexture(const VertexShaderPtr& vertex_shader, const TexturePtr& texture)
{
	auto shader_resource_view = texture->GetShaderResourceView();
	assert(shader_resource_view);
	m_device_context->VSSetShaderResources(0, 1, &shader_resource_view);
}

void DeviceContext::SetTexture(const PixelShaderPtr& pixel_shader, const TexturePtr& texture)
{
	auto shader_resource_view = texture->GetShaderResourceView();
	assert(shader_resource_view);
	m_device_context->PSSetShaderResources(0, 1, &shader_resource_view);
}

void DeviceContext::SetConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& constant_buffer)
{
	auto buffer = constant_buffer->GetBuffer();
	assert(buffer);
	m_device_context->VSSetConstantBuffers(0, 1, &buffer);
}

void DeviceContext::SetConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& constant_buffer)
{
	auto buffer = constant_buffer->GetBuffer();
	assert(buffer);
	m_device_context->PSSetConstantBuffers(0, 1, &buffer);
}

void DeviceContext::SetIndexBuffer(const IndexBufferPtr& index_buffer)
{
	m_device_context->IASetIndexBuffer(index_buffer->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::DrawTriangleList(UINT vertex_count, UINT start_vertex_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_location);
}

void DeviceContext::DrawIndexedTriangleList(UINT index_count, UINT start_index_location, UINT base_vertex_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(index_count, start_index_location, base_vertex_location);
}

void DeviceContext::DrawTriangleStrip(UINT vertex_count, UINT start_vertex_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_location);
}

void DeviceContext::ClearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red, green, blue, alpha };
	m_device_context->ClearRenderTargetView(swap_chain->GetRenderTargetView(), clear_color);
	assert(m_device_context);
	auto render_target_views = swap_chain->GetRenderTargetView();
	assert(render_target_views);
	m_device_context->OMSetRenderTargets(1, &render_target_views, nullptr);
}
