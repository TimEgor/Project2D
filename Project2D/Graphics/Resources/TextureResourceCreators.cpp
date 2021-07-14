#include "TextureResourceCreators.h"

#include <Graphics/GraphicDeviceManager.h>
#include <Graphics/Resources/TextureResources.h>
#include <ResourceManager/ResourceHandler.h>

void PNG_TextureResourceCreator::createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem) {
	size_t dataSize = 0;
	void* data = nullptr;

	loadResourceFromZipFileSystem(resourceID, zipFileSystem, &data, dataSize);

	createResourceFromMemory(handler, data, dataSize);

	delete[] data;
}

void PNG_TextureResourceCreator::createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize) {
	
	SubresourceData initialData{};
	initialData.mem = data;
	initialData.memPitch = dataSize;

	Texture2DReference texture(GraphicDeviceManager::get().getCurrentDevice()->createTexture2DFromMemory(&initialData));
	Texture2DResource* newResource = new Texture2DResource(texture);
	handler.setResource(newResource);
}
