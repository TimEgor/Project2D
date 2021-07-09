#pragma once

#include <Graphics/D3D11/D3D11.h>

#ifdef RENDERING_D3D11

#include <Graphics/GraphicDevice.h>

class D3D11Device final : public GraphicDevice {
private:
	HMODULE d3d11LibHandle = 0;
	HMODULE d3dCompilerLibHandle = 0;
	HMODULE dxgiLibHandle = 0;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	IDXGIFactory* dxgiFactory = nullptr;

	DXGI_FORMAT convertFormat(Format format);
	D3D11_USAGE convertUsage(Usage usage);
	D3D11_BIND_FLAG convertBindFlags(Bind bindFlag);
	D3D11_CPU_ACCESS_FLAG convertCpuAccessFlag(CPUAccess cpuAccess);

	virtual bool init() override;
	virtual void release() override;

public:
	D3D11Device() = default;

	virtual Texture1D_Reference createTexture1D(const Texture1DDesc& desc, const SubresourceData* data) override;
	virtual Texture2D_Reference createTexture2D(const Texture2DDesc& desc, const SubresourceData* data) override;
	virtual Texture3D_Reference createTexture3D(const Texture3DDesc& desc, const SubresourceData* data) override;

	virtual GraphicalDeviceType getGraphicalDeviceType() const override { return D3D11_GraphicalDeviceType; };
};

#endif // RENDERING_D3D11