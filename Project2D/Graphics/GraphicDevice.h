#pragma once

#include <Graphics/GraphicTypes.h>
#include <Multithreading/AtomicCounterObjectBase.h>

typedef AtomicCounterObjetcBaseReference<Texture1D> Texture1D_Reference;
typedef AtomicCounterObjetcBaseReference<Texture2D> Texture2D_Reference;
typedef AtomicCounterObjetcBaseReference<Texture3D> Texture3D_Reference;

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

	virtual Texture1D_Reference createTexture1D(const Texture1DDesc& desc, const SubresourceData* data) = 0;
	virtual Texture2D_Reference createTexture2D(const Texture2DDesc& desc, const SubresourceData* data) = 0;
	virtual Texture3D_Reference createTexture3D(const Texture3DDesc& desc, const SubresourceData* data) = 0;

	virtual GraphicalDeviceType getGraphicalDeviceType() const = 0;
};