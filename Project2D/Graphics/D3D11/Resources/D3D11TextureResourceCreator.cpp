#include "D3D11TextureResourceCreator.h"

#include <ResourceManager/ResourceHandler.h>
#include <ResourceManager/Zip/ZipFileSystem.h>
#include <Graphics/D3D11/Resources/D3D11TextureResource.h>

#include <cassert>

void D3D11_PNG_TextureResourceCreator::createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem) {
	size_t dataSize = 0;
	void* data = nullptr;

	loadResourceFromZipFileSystem(resourceID, zipFileSystem, &data, dataSize);

	createResourceFromMemory(handler, data, dataSize);

	delete[] data;
}

void D3D11_PNG_TextureResourceCreator::createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize) {
	//D3D11TextureResource* resource = new D3D11TextureResource(data, dataSize);
	//handler.setResource(resource);
}
