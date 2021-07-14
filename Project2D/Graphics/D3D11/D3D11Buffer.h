#pragma once

#include <Graphics/D3D11/D3D11Device.h>

class D3D11GpuBuffer final : public GPUBuffer {
	friend D3D11Device;

private:
	ID3D11Buffer* buffer = nullptr;

	virtual void release() override { D3D11ObjectRelease(buffer); }

public:
	D3D11GpuBuffer(D3D11Device& device, const GPUBufferDesc& desc)
		: GPUBuffer(device, desc) {}

	ID3D11Buffer** getBufferNativeHandle() { return &buffer; }
};