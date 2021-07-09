#include "D3D11MaterialResourceCreator.h"

#include <ResourceManager/Zip/ZipFileSystem.h>
#include <ResourceManager/ResourceManager.h>
#include <Utilities/PugiXML/pugixml.hpp>
#include <Utilities/XML_TypesReader/XML_TypesReader.h>
#include <Graphics/D3D11/Resources/D3D11MaterialResource.h>
#include <Graphics/D3D11/Resources/D3D11ShaderResource.h>

#include <cassert>

//void float3x3PropertyTranspose(void* address) {
//	DirectX::XMMATRIX matrix = DirectX::XMLoadFloat3x3((DirectX::XMFLOAT3X3*)address);
//	matrix = XMMatrixTranspose(matrix);
//	DirectX::XMStoreFloat3x3((DirectX::XMFLOAT3X3*)address, matrix);
//}
//
//void float4x4PropertyTranspose(void* address) {
//	DirectX::XMMATRIX matrix = DirectX::XMLoadFloat4x4((DirectX::XMFLOAT4X4*)address);
//	matrix = XMMatrixTranspose(matrix);
//	DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)address, matrix);
//}
//
//void fillMaterialData(const pugi::xml_node& node, uint8_t* data, D3D11ShaderMaterialProperties& propertiesMap) {
//	const D3D11ShaderMaterialProperties::PropertiesMap& properties = propertiesMap.getPropertiesMap();
//
//	for (auto propertyNode = node.first_child(); propertyNode; propertyNode = propertyNode.next_sibling()) {
//		auto findNodeIter = properties.find(ResourceName(propertyNode.name()).hash());
//		assert(findNodeIter != properties.end());
//
//		void* address = data + findNodeIter->second.offset;
//
//		HLSL_Type type = findNodeIter->second.type;
//		switch (type) {
//		case HLSL_Bool:
//			*(float*)address = propertyNode.text().as_bool();
//			break;
//
//		case HLSL_Int:
//			*(int*)address = propertyNode.text().as_int();
//			break;
//
//		case HLSL_UInt:
//			*(unsigned int*)address = propertyNode.text().as_uint();
//			break;
//
//		case HLSL_DWord:
//			*(DWORD*)address = (DWORD)propertyNode.text().as_int();
//			break;
//
//		case HLSL_Float:
//			*(float*)address = propertyNode.text().as_float();
//			break;
//
//		case HLSL_Double:
//			*(double*)address = propertyNode.text().as_double();
//			break;
//
//		case HLSL_Float2:
//			xmlReadVector_XMFLOAT2(propertyNode, *(DirectX::XMFLOAT2*)address);
//			break;
//
//		case HLSL_Float3:
//			xmlReadVector_XMFLOAT3(propertyNode, *(DirectX::XMFLOAT3*)address);
//			break;
//
//		case HLSL_Float4:
//			xmlReadVector_XMFLOAT4(propertyNode, *(DirectX::XMFLOAT4*)address);
//			break;
//
//		case HLSL_Float3x3:
//			xmlReadVector_XMFLOAT3X3(propertyNode, *(DirectX::XMFLOAT3X3*)address);
//			float3x3PropertyTranspose(address);
//			break;
//
//		case HLSL_Float4x4:
//			xmlReadVector_XMFLOAT4X4(propertyNode, *(DirectX::XMFLOAT4X4*)address);
//			float4x4PropertyTranspose(address);
//			break;
//
//		default:
//			break;
//		}
//	}
//}
//
//void D3D11MaterialResourceCreator::createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem) {
//	size_t dataSize = 0;
//	void* data = nullptr;
//
//	loadResourceFromZipFileSystem(resourceID, zipFileSystem, &data, dataSize);
//
//	createResourceFromMemory(handler, data, dataSize);
//
//	delete[] data;
//}
//
//void D3D11MaterialResourceCreator::createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize) {
//	ResourceManager& resourceManager = ResourceManager::get();
//
//	pugi::xml_document doc;
//	pugi::xml_parse_result result = doc.load_buffer(data, dataSize);
//
//	assert(result.status == pugi::xml_parse_status::status_ok);
//
//	pugi::xml_node materialNode = doc.child("Material");
//	assert(materialNode);
//
//	ResourceName shaderResName;
//	pugi::xml_node shaderNode;
//
//	size_t bufferSize = 0;
//
//	//PixelShader
//	if (shaderNode = materialNode.child("PixelShader")) {
//		shaderResName = shaderNode.text().as_string();
//	}
//	else {
//		shaderResName = "generic/SpriteDefaultPixelShader.pshader";
//	}
//
//	ResourceReference pixelShader = resourceManager.getResourceFromArchive(shaderResName);
//	void* pixelShaderProperties = nullptr;
//
//	D3D11PixelShaderResource& pixelShaderResource = pixelShader.getResource<D3D11PixelShaderResource>();
//	bufferSize = pixelShaderResource.getProperties().getPropertyDataSize();
//	if (bufferSize) {
//		pixelShaderProperties = new uint8_t[bufferSize];
//		fillMaterialData(materialNode.child("PixelShaderData"), (uint8_t*)pixelShaderProperties, pixelShaderResource.getProperties());
//	}
//
//	//VertexShader
//	if (shaderNode = materialNode.child("VertexShader")) {
//		shaderResName = shaderNode.text().as_string();
//	}
//	else {
//		shaderResName = "generic/SpriteDefaultVertexShader.vshader";
//	}
//
//	ResourceReference vertexShader = resourceManager.getResourceFromArchive(shaderResName);
//	void* vertexShaderProperties = nullptr;
//
//	D3D11VertexShaderResource& vertexShaderResource = vertexShader.getResource<D3D11VertexShaderResource>();
//	bufferSize = vertexShaderResource.getProperties().getPropertyDataSize();
//	if (bufferSize) {
//		vertexShaderProperties = new uint8_t[bufferSize];
//		fillMaterialData(materialNode.child("VertexShaderData"), (uint8_t*)vertexShaderProperties, vertexShaderResource.getProperties());
//	}
//
//	D3D11MaterialResource* materialResource = new D3D11MaterialResource(pixelShader, vertexShader, pixelShaderProperties, vertexShaderProperties);
//	handler.setResource(materialResource);
//
//	if (pixelShaderProperties) {
//		delete[] pixelShaderProperties;
//	}
//
//	if (vertexShaderProperties) {
//		delete[] vertexShaderProperties;
//	}
//}
