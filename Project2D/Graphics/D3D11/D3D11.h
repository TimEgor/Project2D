#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

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
	~D3D11() { release(); }

	static D3D11& get();

	bool init();
	void release();

	ID3D11Device* getDevice() { return device; }
	ID3D11DeviceContext* getDeviceContext() { return deviceContext; }

	IDXGIFactory* getDXGIFactory() { return dxgiFactory; }
	IDXGISwapChain* getSwapChain() { return swapChain; }
};