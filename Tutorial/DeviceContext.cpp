#include <cassert>
#include <d3d11.h>
#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context)
	: m_device_context(device_context)
{
}

ID3D11DeviceContext* DeviceContext::GetDeviceContext()
{
	return m_device_context;
}

void DeviceContext::SetViewportSize(UINT width, UINT height)
{
	// 뷰포트 구조체
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// 뷰포트 설정
	m_device_context->RSSetViewports(1, &viewport);
}

void DeviceContext::SetVertexBuffer(VertexBuffer* vertex_buffer)
{
	UINT stride = vertex_buffer->GetVertexSize();
	UINT offset = 0;
	auto vertex_buffers = vertex_buffer->GetBuffer();
	assert(vertex_buffers);
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffers, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->GetInputLayout());
}

void DeviceContext::SetVertexShader(VertexShader* vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->GetVertexShader(), nullptr, 0);
}

void DeviceContext::SetPixelShader(PixelShader* pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->GetPixelShader(), nullptr, 0);
}

void DeviceContext::SetConstantBuffer(ConstantBuffer* constant_buffer, VertexShader* vertex_shader)
{
	auto buffer = constant_buffer->GetBuffer();
	assert(buffer);
	m_device_context->VSSetConstantBuffers(0, 1, &buffer);
}

void DeviceContext::SetConstantBuffer(ConstantBuffer* constant_buffer, PixelShader* pixel_shader)
{
	auto buffer = constant_buffer->GetBuffer();
	assert(buffer);
	m_device_context->PSSetConstantBuffers(0, 1, &buffer);
}

void DeviceContext::SetIndexBuffer(IndexBuffer* index_buffer)
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

void DeviceContext::ClearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red, green, blue, alpha };
	m_device_context->ClearRenderTargetView(swap_chain->GetRenderTargetView(), clear_color);
	assert(m_device_context);
	auto render_target_views = swap_chain->GetRenderTargetView();
	assert(render_target_views);
	m_device_context->OMSetRenderTargets(1, &render_target_views, nullptr);
}

void DeviceContext::Release()
{
	assert(m_device_context);
	m_device_context->Release();
	delete this;
}
