#pragma once

#include <d3d11.h>

class VertexBuffer
{
public:
	void Load(void* vertices, UINT vertex_size, UINT list_size, void* shader_byte_code, UINT shader_byte_size);
	void Release();

	ID3D11Buffer* GetBuffer() const;
	ID3D11InputLayout* GetInputLayout() const;
	UINT GetVertexSize() const;

private:
	UINT m_vertex_size = 0;
	UINT m_list_size = 0;

	ID3D11Buffer* m_buffer = nullptr;
	ID3D11InputLayout* m_input_layout = nullptr;
};

