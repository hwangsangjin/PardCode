#include <cassert>
#include "VertexBuffer.h"
#include "Graphics.h"

void VertexBuffer::Load(void* vertices, UINT vertex_size, UINT list_size, void* shader_byte_code, size_t shader_byte_size)
{
    if (m_buffer)
        m_buffer->Release();
    if (m_input_layout)
        m_input_layout->Release();

    // 버퍼 구조체
    D3D11_BUFFER_DESC buff_desc = {};
    buff_desc.Usage = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth = vertex_size * list_size;
    buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buff_desc.CPUAccessFlags = 0;
    buff_desc.MiscFlags = 0;

    // 서브리소스 구조체
    D3D11_SUBRESOURCE_DATA init_data = {};
    init_data.pSysMem = vertices;

    m_vertex_size = vertex_size;
    m_list_size = list_size;

    // 버퍼 생성
    Graphics::GetInstance()->GetD3DDevice()->CreateBuffer(&buff_desc, &init_data, &m_buffer);
    assert(m_buffer);

    // 입력 레이아웃 구조체
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT layout_size = ARRAYSIZE(layout);

    // 입력 레이아웃 생성
    Graphics::GetInstance()->GetD3DDevice()->CreateInputLayout(layout, layout_size, shader_byte_code, shader_byte_size, &m_input_layout);
    assert(m_input_layout);
}

void VertexBuffer::Release()
{
    m_input_layout->Release();
    m_buffer->Release();
    delete this;
}

ID3D11Buffer* VertexBuffer::GetBuffer() const
{
    return m_buffer;
}

ID3D11InputLayout* VertexBuffer::GetInputLayout() const
{
    return m_input_layout;
}

UINT VertexBuffer::GetVertexSize() const
{
    return m_vertex_size;
}
