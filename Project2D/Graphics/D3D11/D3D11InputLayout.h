#pragma once

#include <Graphics/GraphicTypes.h>
#include <Graphics/D3D11/D3D11.h>

class D3D11Device;

class D3D11InputLayout final : public InputLayout {
private:
	ID3D11InputLayout* inputLayer = nullptr;

	virtual void release() override { D3D11ObjectRelease(inputLayer); }

public:
	D3D11InputLayout(D3D11Device& device, size_t id)
		: InputLayout(device, id) {}

	ID3D11InputLayout** getInputLayerNativeHandle() { return &inputLayer; }
};