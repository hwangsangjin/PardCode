#pragma once

#include <d3d11.h>

class DeviceContext;

class ConstantBuffer
{
public:
	void Load(void* buffer, UINT buffer_size);
	void Update(DeviceContext* device_context, void* buffer);
	void Release();

	ID3D11Buffer* GetBuffer() const;

private:
	ID3D11Buffer* m_buffer = nullptr;
};

