#include "ShaderResourceCreators.h"

#include <Graphics/GraphicDeviceManager.h>
#include <Graphics/Resources/ShaderResources.h>
#include <ResourceManager/ResourceHandler.h>

void VertexShaderResourceCreator::createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem) {
	size_t dataSize = 0;
	void* data = nullptr;

	loadResourceFromZipFileSystem(resourceID, zipFileSystem, &data, dataSize);

	createResourceFromMemory(handler, data, dataSize);

	delete[] data;
}

void VertexShaderResourceCreator::createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize) {
	VertexShaderReference shader(GraphicDeviceManager::get().getCurrentDevice()->createVertexShaderFromStrSource(data, dataSize));
	VertexShaderResource* newResource = new VertexShaderResource(shader);
	handler.setResource(newResource);
}

void PixelShaderResourceCreator::createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem) {
	size_t dataSize = 0;
	void* data = nullptr;

	loadResourceFromZipFileSystem(resourceID, zipFileSystem, &data, dataSize);

	createResourceFromMemory(handler, data, dataSize);

	delete[] data;
}

void PixelShaderResourceCreator::createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize) {
	PixelShaderReference shader(GraphicDeviceManager::get().getCurrentDevice()->createPixelShaderFromStrSource(data, dataSize));
	PixelShaderResource * newResource = new PixelShaderResource(shader);
	handler.setResource(newResource);
}