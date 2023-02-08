#pragma once

#include <d3d11.h>

class SwapChain;
class DeviceContext;

class Graphics
{
public:
	bool Initialize();
	bool Release();

	static Graphics* GetInstance();
	ID3D11Device* GetD3DDevice() const;
	IDXGIFactory* GetDXGIFactory() const;
	SwapChain* CreateSwapChain();
	DeviceContext* GetDeviceContext() const;

private:
	ID3D11Device* m_d3d_device = nullptr;
	D3D_FEATURE_LEVEL m_feature_level;
	DeviceContext* m_device_context = nullptr;
	IDXGIDevice* m_dxgi_device = nullptr;
	IDXGIAdapter* m_dxgi_adapter = nullptr;
	IDXGIFactory* m_dxgi_factory = nullptr;
};

