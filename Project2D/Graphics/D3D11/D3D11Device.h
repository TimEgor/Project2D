#pragma once

#include <Graphics/D3D11/D3D11.h>

#ifdef RENDERING_D3D11

#include <Graphics/GraphicDevice.h>

#include <unordered_map>

class D3D11InputLayout;

class D3D11Device final : public GraphicDevice {
private:
	enum HLSL_Type {
		HLSL_Bool,
		HLSL_Int,
		HLSL_UInt,
		HLSL_DWord,
		HLSL_Float,
		HLSL_Double,
		HLSL_Float2,
		HLSL_Float3,
		HLSL_Float4,
		HLSL_Float3x3,
		HLSL_Float4x4
	};

	const std::unordered_map<std::string, HLSL_Type> hlslTypes = {
		{ "bool", HLSL_Bool },
		{ "int", HLSL_Int },
		{ "uint", HLSL_UInt },
		{ "dword", HLSL_DWord },
		{ "float", HLSL_Float },
		{ "double", HLSL_Double },
		{ "float2", HLSL_Float2 },
		{ "float3", HLSL_Float3 },
		{ "float4", HLSL_Float4 },
		{ "float3x3", HLSL_Float3x3 },
		{ "float4x4", HLSL_Float4x4 }
	};

private:
	std::unordered_map<size_t, D3D11InputLayout*> inputLayouts;

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

	void outputShaderCompilingError(ID3D10Blob* errorBlob);

	virtual bool init() override;
	virtual void release() override;

public:
	D3D11Device() = default;

	virtual Texture1DReference createTexture1D(const Texture1DDesc& desc, const SubresourceData* data) override;
	virtual Texture2DReference createTexture2D(const Texture2DDesc& desc, const SubresourceData* data) override;
	virtual Texture3DReference createTexture3D(const Texture3DDesc& desc, const SubresourceData* data) override;

	virtual Texture2DReference createTexture2DFromMemory(void* data, size_t dataSize) override;

	virtual GPUBufferReference createGPUBuffer(const GPUBufferDesc& desc, const SubresourceData* data) override;

	virtual VertexShaderReference createVertexShaderFromCompiledCode(void* data, size_t size) override;
	virtual PixelShaderReference createPixelShaderFromCompiledCode(void* data, size_t size) override;
	virtual VertexShaderReference createVertexShaderFromStrSource(void* data, size_t size) override;
	virtual PixelShaderReference createPixelShaderFromStrSource(void* data, size_t size) override;

	virtual InputLayoutReference createInputLayout(const InputLayoutDesc& desc, VertexShaderReference vertexShader) override;

	virtual PipelineStateReference createPipelineState(const PipelineStateDesc& desc) override;

	virtual GraphicalDeviceType getGraphicalDeviceType() const override { return D3D11_GraphicalDeviceType; };
};

#endif // RENDERING_D3D11