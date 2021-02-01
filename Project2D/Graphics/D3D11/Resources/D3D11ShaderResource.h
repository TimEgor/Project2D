#pragma once

#include <ResourceManager/Resource.h>
#include <Graphics/D3D11/D3D11.h>
#include <Graphics/D3D11/Resources/D3D11ShaderMaterialProperties.h>

class D3D11PixelShaderResource final : public Resource {
private:
	D3D11ShaderMaterialProperties properties;
	ID3D11PixelShader* pixelShader;

public:
	D3D11PixelShaderResource(void* shaderSourceData, size_t shaderSourceDataSize, const D3D11ShaderMaterialProperties& properties);
	~D3D11PixelShaderResource();

	ID3D11PixelShader* getPixelShader() { return pixelShader; }
	D3D11ShaderMaterialProperties& getProperties() { return properties; }

	virtual ResourceType getResourceType() override { return D3D11PixelShaderResourceType; }
};

class D3D11VertexShaderResource final : public Resource {
private:
	D3D11ShaderMaterialProperties properties;
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;

public:
	D3D11VertexShaderResource(void* shaderSourceData, size_t shaderSourceDataSize, const D3D11ShaderMaterialProperties& properties);
	~D3D11VertexShaderResource();

	ID3D11VertexShader* getVertexShader() { return vertexShader; }
	ID3D11InputLayout* getInputLayout() { return inputLayout; }
	D3D11ShaderMaterialProperties& getProperties() { return properties; }

	virtual ResourceType getResourceType() override { return D3D11VertexShaderResourceType; }
};