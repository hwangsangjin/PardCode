#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT buffer_size, Graphics* graphics);
	~ConstantBuffer();

	void Update(DeviceContextPtr device_context, void* buffer);

	ID3D11Buffer* GetBuffer() const;

private:
	ID3D11Buffer* m_buffer = nullptr;
	Graphics* m_graphics = nullptr;
};

