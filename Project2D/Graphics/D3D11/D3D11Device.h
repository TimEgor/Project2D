#pragma once

#include <Graphics/D3D11/D3D11.h>

#ifdef RENDERING_D3D11

#include <Graphics/GraphicDevice.h>

#include <unordered_map>

class D3D11InputLayout;

class D3D11Device final : public GraphicDevice {
private:
	std::unordered_map<size_t, D3D11InputLayout*> layouts;

	HMODULE d3d11LibHandle = 0;
	HMODULE d3dCompilerLibHandle = 0;
	HMODULE dxgiLibHandle = 0;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	IDXGIFactory* dxgiFactory = nullptr;

	DXGI_FORMAT convertFormatToD3D11(Format format);
	Format convertFormatFromD3D11(DXGI_FORMAT format);
	D3D11_USAGE convertUsageToD3D11(Usage usage);
	Usage convertUsageFromD3D11(D3D11_USAGE usage);
	D3D11_BIND_FLAG convertBindFlagToD3D11(Bind bindFlag);
	Bind convertBindFlagFromD3D11(D3D11_BIND_FLAG bindFlag);
	D3D11_CPU_ACCESS_FLAG convertCpuAccessFlagToD3D11(CPUAccess cpuAccess);
	CPUAccess convertCpuAccessFlagFromD3D11(D3D11_CPU_ACCESS_FLAG cpuAccess);
	uint32_t convertBindFlagsToD3D11(uint32_t bindFlags);
	uint32_t convertBindFlagsFromD3D11(uint32_t bindFlags);
	uint32_t convertCpuAccessFlagsToD3D11(uint32_t cpuAccessFlags);
	uint32_t convertCpuAccessFlagsFromD3D11(uint32_t cpuAccessFlags);

	DXGI_FORMAT convertInputLayoutTypeToDXGI(InputLayoutElementType type, uint32_t componentsNum);
	void convertInputLayoutDescToD3D11(const std::vector<InputLayoutElement>& desc, std::vector<D3D11_INPUT_ELEMENT_DESC>& result);

	virtual bool init() override;
	virtual void release() override;

public:
	D3D11Device() = default;

	virtual Texture1DReference createTexture1D(const Texture1DDesc& desc, const SubresourceData* data) override;
	virtual Texture2DReference createTexture2D(const Texture2DDesc& desc, const SubresourceData* data) override;
	virtual Texture3DReference createTexture3D(const Texture3DDesc& desc, const SubresourceData* data) override;

	virtual Texture2DReference createTexture2DFromMemory(const SubresourceData* data) override;

	virtual GPUBufferReference createGPUBuffer(const GPUBufferDesc& desc, const SubresourceData* data) override;

	virtual VertexShaderReference createVertexShaderFromCompiledCode(void* data, size_t size) override;
	virtual PixelShaderReference createPixelShaderFromCompiledCode(void* data, size_t size) override;
	virtual VertexShaderReference createVertexShaderFromStrSource(void* data, size_t size) override;
	virtual PixelShaderReference createPixelShaderFromStrSource(void* data, size_t size) override;

	virtual InputLayerReference createInputLayout(const InputLayoutDesc& desc, VertexShaderReference vertexShader) override;

	virtual PipelineStateReference createPipelineState(const PipelineStateDesc& desc) override;

	virtual GraphicalDeviceType getGraphicalDeviceType() const override { return D3D11_GraphicalDeviceType; };
};

#endif // RENDERING_D3D11