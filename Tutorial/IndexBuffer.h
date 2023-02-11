#pragma once

#include <d3d11.h>

class IndexBuffer
{
public:
	void Load(void* indices, UINT index_count);
	void Release();

	ID3D11Buffer* GetBuffer() const;
	UINT GetIndexCount() const;

private:
	UINT m_index_count = 0;

	ID3D11Buffer* m_buffer = nullptr;
};

