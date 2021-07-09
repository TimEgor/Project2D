#include "D3D11MaterialResource.h"

#include <Graphics/D3D11/Resources/D3D11ShaderResource.h>
#include <Graphics/D3D11/Resources/D3D11ShaderMaterialProperties.h>
//
//D3D11MaterialResource::D3D11MaterialResource(ResourceReference pixelShader, ResourceReference vertexShader,
//	void* pixelShaderPropertyData, void* vertexShaderPropertyData)
//	: pixelShader(pixelShader), vertexShader(vertexShader),
//	pixelShaderProperties(nullptr), vertexShaderProperties(nullptr) {
//	D3D11& d3d11 = D3D11::get();
//
//	D3D11_BUFFER_DESC bufferDesc{};
//	bufferDesc.Usage = D3D11_USAGE_STAGING;
//	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
//	bufferDesc.MiscFlags = 0;
//	bufferDesc.StructureByteStride = 0;
//
//	D3D11_SUBRESOURCE_DATA subresourceData{};
//
//	if (pixelShaderPropertyData) {
//		D3D11PixelShaderResource& pixelShaderResource = pixelShader.getResource<D3D11PixelShaderResource>();
//		bufferDesc.ByteWidth = pixelShaderResource.getProperties().getPropertyDataSize();
//		subresourceData.pSysMem = pixelShaderPropertyData;
//		d3d11.getDevice()->CreateBuffer(&bufferDesc, &subresourceData, &pixelShaderProperties);
//	}
//
//	if (vertexShaderPropertyData) {
//		D3D11VertexShaderResource& vertexShaderResource = vertexShader.getResource<D3D11VertexShaderResource>();
//		bufferDesc.ByteWidth = vertexShaderResource.getProperties().getPropertyDataSize();
//		subresourceData.pSysMem = vertexShaderPropertyData;
//		d3d11.getDevice()->CreateBuffer(&bufferDesc, &subresourceData, &vertexShaderProperties);
//	}
//}
//
//D3D11MaterialResource::~D3D11MaterialResource() {
//	D3D11ObjectRelease(pixelShaderProperties);
//	D3D11ObjectRelease(vertexShaderProperties);
//}
