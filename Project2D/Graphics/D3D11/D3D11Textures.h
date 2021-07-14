#pragma once

#include <Graphics/D3D11/D3D11Device.h>

class D3D11Texture1D final : public Texture1D {
	friend D3D11Device;

private:
	ID3D11Texture1D* texture = nullptr;

	virtual void release() override { D3D11ObjectRelease(texture); }

public:
	D3D11Texture1D(D3D11Device& device, const Texture1DDesc& desc)
		: Texture1D(device, desc) {}

	ID3D11Texture1D** getTextureNativeHandle() { return &texture; }
};

class D3D11Texture2D final : public Texture2D {
	friend D3D11Device;

private:
	ID3D11Texture2D* texture = nullptr;

	virtual void release() override { D3D11ObjectRelease(texture); }

public:
	D3D11Texture2D(D3D11Device& device, const Texture2DDesc& desc)
		: Texture2D(device, desc) {}

	ID3D11Texture2D** getTextureNativeHandle() { return &texture; }
};

class D3D11Texture3D final : public Texture3D {
	friend D3D11Device;

private:
	ID3D11Texture3D* texture = nullptr;

	virtual void release() override { D3D11ObjectRelease(texture); }

public:
	D3D11Texture3D(D3D11Device& device, const Texture3DDesc& desc)
		: Texture3D(device, desc) {}

	ID3D11Texture3D** getTextureNativeHandle() { return &texture; }
};