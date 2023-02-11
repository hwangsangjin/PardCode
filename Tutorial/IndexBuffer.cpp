#include <cassert>
#include "IndexBuffer.h"
#include "Graphics.h"

void IndexBuffer::Load(void* indices, UINT index_count)
{
    if (m_buffer)
        m_buffer->Release();

    // ���� ����ü
    D3D11_BUFFER_DESC buff_desc = {};
    buff_desc.Usage = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth = sizeof(int) * index_count;
    buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buff_desc.CPUAccessFlags = 0;
    buff_desc.MiscFlags = 0;

    // ���긮�ҽ� ����ü
    D3D11_SUBRESOURCE_DATA init_data = {};
    init_data.pSysMem = indices;

    m_index_count = index_count;

    // ���� ����
    Graphics::GetInstance()->GetD3DDevice()->CreateBuffer(&buff_desc, &init_data, &m_buffer);
    assert(m_buffer);
}

void IndexBuffer::Release()
{
    assert(m_buffer);
    m_buffer->Release();
    delete this;
}

ID3D11Buffer* IndexBuffer::GetBuffer() const
{
    return m_buffer;
}

UINT IndexBuffer::GetIndexCount() const
{
    return m_index_count;
}
