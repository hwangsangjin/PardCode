#pragma once

#include <d3d11.h>

class VertexShader
{
public:
	void Initialize(const void* shader_byte_code, size_t byte_code_size);
	void Release();

	ID3D11VertexShader* GetVertexShader() const;

private:
	ID3D11VertexShader* m_vertex_shader = nullptr;
};

