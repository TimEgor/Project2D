#include "ResourceCreator.h"

#include <ResourceManager/NativeFileSystem.h>
#include <ResourceManager/Zip/ZipFileSystem.h>
#include <ResourceManager/ResourceHandler.h>
#include <ResourceManager/ResourceName.h>

#include <cassert>

void loadResourceFromZipFileSystem(ResourceID resourceID, ZipFileSystem& zipFileSystem, void** data, size_t& size) {
	size = 0;
	bool checker = false;
	checker = zipFileSystem.getResourceSize(resourceID, size);
	assert(checker);

	*data = new uint8_t[size];
	checker = zipFileSystem.getResourceData(resourceID, *data, size);
	assert(checker);
}

void DefaultResourceCreator::createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem) {
	size_t resourceSize = 0;
	bool checker = false;
	checker = zipFileSystem.getResourceSize(resourceID, resourceSize);
	assert(checker);

	void* data = new uint8_t[resourceSize];
	checker = zipFileSystem.getResourceData(resourceID, data, resourceSize);
	assert(checker);

	createResourceFromMemory(handler, data, resourceSize);
}

void DefaultResourceCreator::createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize) {
	DefaultResource* defaultResource = new DefaultResource(data, dataSize);
	handler.setResource(defaultResource);
}
