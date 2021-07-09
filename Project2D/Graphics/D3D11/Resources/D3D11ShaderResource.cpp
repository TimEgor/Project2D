#include "D3D11ShaderResource.h"

#include <Graphics/D3D11/D3D11Verteces.h>

//D3D11PixelShaderResource::D3D11PixelShaderResource(void* shaderSourceData, size_t shaderSourceDataSize, const D3D11ShaderMaterialProperties& properties) : properties(properties) {
//	D3D11& d3d11 = D3D11::get();
//	d3d11.getDevice()->CreatePixelShader(shaderSourceData, shaderSourceDataSize, nullptr, &pixelShader);
//}
//
//D3D11PixelShaderResource::~D3D11PixelShaderResource() {
//	D3D11ObjectRelease(pixelShader);
//}
//
//D3D11VertexShaderResource::D3D11VertexShaderResource(void* shaderSourceData, size_t shaderSourceDataSize, const D3D11ShaderMaterialProperties& properties) : properties(properties) {
//	D3D11& d3d11 = D3D11::get();
//	ID3D11Device* device = d3d11.getDevice();
//	device->CreateVertexShader(shaderSourceData, shaderSourceDataSize, nullptr, &vertexShader);
//	device->CreateInputLayout(D3D11SpriteVertex::desc, sizeof(D3D11SpriteVertex::desc) / sizeof(D3D11_INPUT_ELEMENT_DESC), shaderSourceData, shaderSourceDataSize, &inputLayout);
//}
//
//D3D11VertexShaderResource::~D3D11VertexShaderResource() {
//	D3D11ObjectRelease(vertexShader);
//	D3D11ObjectRelease(inputLayout);
//}
