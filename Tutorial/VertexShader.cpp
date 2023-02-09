#include <cassert>
#include "VertexShader.h"
#include "Graphics.h"

void VertexShader::Initialize(const void* shader_byte_code, size_t byte_code_size)
{
	Graphics::GetInstance()->GetD3DDevice()->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_vertex_shader);
	assert(m_vertex_shader);
}

void VertexShader::Release()
{
}

ID3D11VertexShader* VertexShader::GetVertexShader() const
{
	return m_vertex_shader;
}
