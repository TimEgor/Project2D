#pragma once

#include <Graphics/D3D11/D3D11.h>
#include <ResourceManager/Resource.h>

class D3D11TextureResource final : public Resource {
private:
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* shaderResourceView;

public:
	D3D11TextureResource(void* data, size_t dataSize);
	~D3D11TextureResource();

	ID3D11Texture2D* getTexture() { return texture; }
	ID3D11ShaderResourceView* getShaderResoruceView() { return shaderResourceView; }

	virtual ResourceType getResourceType() override { return D3D11TextureResourceType; }
};