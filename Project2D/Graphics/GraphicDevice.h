#pragma once

#include <Graphics/GraphicTypes.h>
#include <Graphics/PipelineState.h>

class GraphicDeviceManager;

enum GraphicalDeviceType {
	D3D11_GraphicalDeviceType
};

class GraphicDevice {
	friend GraphicDeviceManager;

protected:
	virtual bool init() = 0;
	virtual void release() = 0;

public:
	virtual ~GraphicDevice() {}

	virtual Texture1DReference createTexture1D(const Texture1DDesc& desc, const SubresourceData* data) = 0;
	virtual Texture2DReference createTexture2D(const Texture2DDesc& desc, const SubresourceData* data) = 0;
	virtual Texture3DReference createTexture3D(const Texture3DDesc& desc, const SubresourceData* data) = 0;

	virtual Texture2DReference createTexture2DFromMemory(void* data, size_t dataSize) = 0;

	virtual GPUBufferReference createGPUBuffer(const GPUBufferDesc& desc, const SubresourceData* data) = 0;

	virtual VertexShaderReference createVertexShaderFromCompiledCode(void* data, size_t size) = 0;
	virtual PixelShaderReference createPixelShaderFromCompiledCode(void* data, size_t size) = 0;
	virtual VertexShaderReference createVertexShaderFromStrSource(void* data, size_t size) = 0;
	virtual PixelShaderReference createPixelShaderFromStrSource(void* data, size_t size) = 0;

	virtual InputLayoutReference createInputLayout(const InputLayoutDesc& desc, VertexShaderReference vertexShader) = 0;

	virtual PipelineStateReference createPipelineState(const PipelineStateDesc& desc) = 0;

	virtual GraphicalDeviceType getGraphicalDeviceType() const = 0;
}; 