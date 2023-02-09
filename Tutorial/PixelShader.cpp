#include <cassert>
#include "PixelShader.h"
#include "Graphics.h"

void PixelShader::Initialize(const void* shader_byte_code, size_t byte_code_size)
{
    Graphics::GetInstance()->GetD3DDevice()->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_pixel_shader);
    assert(m_pixel_shader);
}

void PixelShader::Release()
{
    m_pixel_shader->Release();
    delete this;
}

ID3D11PixelShader* PixelShader::GetPixelShader() const
{
    return m_pixel_shader;
}
