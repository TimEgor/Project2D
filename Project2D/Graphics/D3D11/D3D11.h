#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>
#include <dxgi.h>

#define D3D11ObjectRelease(object) \
	if (object) { \
		object->Release(); \
		object = nullptr; \
	}

class D3D11 final {
private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGIFactory* dxgiFactory;
	IDXGISwapChain* swapChain;

	D3D11();

public:
	static D3D11& get();

	bool init();
	void release();

	ID3D11Device* getDevice() { return device; }
	ID3D11DeviceContext* getDeviceContext() { return deviceContext; }

	IDXGIFactory* getDXGIFactory() { return dxgiFactory; }
	IDXGISwapChain* getSwapChain() { return swapChain; }
};