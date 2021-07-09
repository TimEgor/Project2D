#include "D3D11ShaderResourceCreator.h"

#include <ResourceManager/Zip/ZipFileSystem.h>
#include <ResourceManager/ResourceHandler.h>
#include <Graphics/D3D11/Resources/D3D11ShaderResource.h>

#include <cassert>

//void outputShaderCompilingError(ID3D10Blob* error) {
//	OutputDebugStringA((char*)error->GetBufferPointer());
//	D3D11ObjectRelease(error);
//}
//
//void D3D11PixelShaderResourceCreator::createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem) {
//	void* data;
//	size_t dataSize;
//
//	loadResourceFromZipFileSystem(resourceID, zipFileSystem, &data, dataSize);
//
//	createResourceFromMemory(handler, data, dataSize);
//
//	delete[] data;
//}
//
//void D3D11PixelShaderResourceCreator::createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize) {
//	ID3D10Blob* shaderBlob = nullptr;
//	ID3D10Blob* errorBlob = nullptr;
//
//	//PS
//	if (FAILED(D3DCompile(data, dataSize, NULL, NULL, NULL, "PS", "ps_5_0", NULL, NULL, &shaderBlob, &errorBlob))) {
//		outputShaderCompilingError(errorBlob);
//		assert(false && "Pixel shader hasn't been compiled !!!");
//	}
//
//	D3D11PixelShaderResource* shaderResource = new D3D11PixelShaderResource(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
//		D3D11ShaderMaterialProperties(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize()));
//	handler.setResource(shaderResource);
//
//	D3D11ObjectRelease(shaderBlob);
//	D3D11ObjectRelease(errorBlob);
//}
//
//void D3D11VertexShaderResourceCreator::createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem) {
//	void* data;
//	size_t dataSize;
//
//	loadResourceFromZipFileSystem(resourceID, zipFileSystem, &data, dataSize);
//
//	createResourceFromMemory(handler, data, dataSize);
//
//	delete[] data;
//}
//
//void D3D11VertexShaderResourceCreator::createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize) {
//	ID3D10Blob* shaderBlob = nullptr;
//	ID3D10Blob* errorBlob = nullptr;
//
//	//VS
//	if (FAILED(D3DCompile(data, dataSize, NULL, NULL, NULL, "VS", "vs_5_0", NULL, NULL, &shaderBlob, &errorBlob))) {
//		outputShaderCompilingError(errorBlob);
//		assert(false && "Vertex shader hasn't been compiled !!!");
//	}
//
//	D3D11VertexShaderResource* shaderResource = new D3D11VertexShaderResource(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
//		D3D11ShaderMaterialProperties(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize()));
//	handler.setResource(shaderResource);
//
//	D3D11ObjectRelease(shaderBlob);
//	D3D11ObjectRelease(errorBlob);
//}
//
//void D3D11BinaryPixelShaderResourceCreator::createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem) {
//	void* data;
//	size_t dataSize;
//
//	loadResourceFromZipFileSystem(resourceID, zipFileSystem, &data, dataSize);
//
//	createResourceFromMemory(handler, data, dataSize);
//
//	delete[] data;
//}
//
//void D3D11BinaryPixelShaderResourceCreator::createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize) {
//	D3D11PixelShaderResource* shaderResource = new D3D11PixelShaderResource(data, dataSize, D3D11ShaderMaterialProperties(data, dataSize));
//	handler.setResource(shaderResource);
//}
//
//void D3D11BinaryVertexShaderResourceCreator::createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem) {
//	void* data;
//	size_t dataSize;
//
//	loadResourceFromZipFileSystem(resourceID, zipFileSystem, &data, dataSize);
//
//	createResourceFromMemory(handler, data, dataSize);
//
//	delete[] data;
//}
//
//void D3D11BinaryVertexShaderResourceCreator::createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize) {
//	D3D11VertexShaderResource* shaderResource = new D3D11VertexShaderResource(data, dataSize, D3D11ShaderMaterialProperties(data, dataSize));
//	handler.setResource(shaderResource);
//}
