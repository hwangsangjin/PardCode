#pragma once

#include <d3d11.h>

class PixelShader
{
public:
	void Initialize(const void* shader_byte_code, size_t byte_code_size);
	void Release();

	ID3D11PixelShader* GetPixelShader() const;

private:
	ID3D11PixelShader* m_pixel_shader = nullptr;
};

