#pragma once

#include <Graphics/PipelineState.h>
#include <Graphics/D3D11/D3D11.h>

class D3D11Device;

class D3D11PipelineState final : public PipelineState {
private:
	virtual void release() override {}

public:
	D3D11PipelineState(D3D11Device& device, const PipelineStateDesc& desc)
		: PipelineState(device, desc) {}
};