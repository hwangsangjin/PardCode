#include <d3d11.h>
#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context)
	: m_device_context(device_context)
{
}

void DeviceContext::ClearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red, green, blue, alpha };
	m_device_context->ClearRenderTargetView(swap_chain->GetRenderTargetView(), clear_color);
	auto render_target_views = swap_chain->GetRenderTargetView();
	m_device_context->OMSetRenderTargets(1, &render_target_views, nullptr);
}

void DeviceContext::SetVertexBuffer(VertexBuffer* vertex_buffer)
{
	UINT stride = vertex_buffer->GetVertexSize();
	UINT offset = 0;
	auto vertex_buffers = vertex_buffer->GetBuffer();
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffers, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->GetInputLayout());
}

void DeviceContext::DrawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::DrawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::SetViewportSize(float width, float height)
{
	// 뷰포트 구조체
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// 뷰포트 설정
	m_device_context->RSSetViewports(1, &viewport);
}

void DeviceContext::SetVertexShader(VertexShader* vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->GetVertexShader(), nullptr, 0);
}

void DeviceContext::Release()
{
	m_device_context->Release();
	delete this;
}
