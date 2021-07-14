#pragma once

#include <Graphics/GraphicTypes.h>
#include <Multithreading/AtomicCounterObjectBase.h>

typedef AtomicCounterObjetcBaseReference<Texture1D> Texture1DReference;
typedef AtomicCounterObjetcBaseReference<Texture2D> Texture2DReference;
typedef AtomicCounterObjetcBaseReference<Texture3D> Texture3DReference;
typedef AtomicCounterObjetcBaseReference<GPUBuffer> GPUBufferReference;

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

	virtual Texture2DReference createTexture2DFromMemory(const SubresourceData* data) = 0;

	virtual GPUBufferReference createGPUBuffer(const GPUBufferDesc& desc, const SubresourceData* data) = 0;

	virtual GraphicalDeviceType getGraphicalDeviceType() const = 0;
};