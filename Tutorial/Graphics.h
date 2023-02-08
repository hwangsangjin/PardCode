#pragma once

#include <d3d11.h>

class Graphics
{
public:
	Graphics() = default;
	~Graphics() = default;

	bool Initialize();
	bool Release();

	static Graphics* GetInstance();

private:
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	ID3D11DeviceContext* m_immediate_context;
};

