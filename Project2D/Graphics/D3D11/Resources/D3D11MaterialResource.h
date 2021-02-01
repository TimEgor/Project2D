#pragma once

#include <ResourceManager/Resource.h>
#include <ResourceManager/ResourceReference.h>
#include <Graphics/D3D11/D3D11.h>

class D3D11MaterialResource final : public Resource {
private:
	ResourceReference pixelShader;
	ResourceReference vertexShader;

	ID3D11Buffer* pixelShaderProperties;
	ID3D11Buffer* vertexShaderProperties;

public:
	D3D11MaterialResource(ResourceReference pixelShader, ResourceReference vertexShader,
		void* pixelShaderPropertyData, void* vertexShaderPropertyData);
	~D3D11MaterialResource();

	ResourceReference getPixelShaderResource() { return pixelShader; }
	ResourceReference getVertexShaderResource() { return vertexShader; }

	ID3D11Buffer* getPixelShaderProperties() { return pixelShaderProperties; }
	ID3D11Buffer* getVertexShaderProperties() { return vertexShaderProperties; }

	virtual ResourceType getResourceType() override { return D3D11MaterialResourceType; }
};