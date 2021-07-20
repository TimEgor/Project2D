#pragma once

#include <Graphics/GraphicTypes.h>
#include <Graphics/D3D11/D3D11.h>

class D3D11Device;

class D3D11GpuBuffer final : public GPUBuffer {
private:
	ID3D11Buffer* buffer = nullptr;

	virtual void release() override { D3D11ObjectRelease(buffer); }

public:
	D3D11GpuBuffer(D3D11Device& device, const GPUBufferDesc& desc)
		: GPUBuffer(device, desc) {}

	ID3D11Buffer** getBufferNativeHandle() { return &buffer; }
};