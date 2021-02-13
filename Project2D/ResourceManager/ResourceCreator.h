#pragma once

#include <ResourceManager/Resource.h>
#include <ResourceManager/ResourceCreatorType.h>

class ResourceHandler;
class ResourceName;
class ZipFileSystem;
class NativeFileSystem;

void loadResourceFromZipFileSystem(ResourceID resourceID, ZipFileSystem& zipFileSystem, void** data, size_t& size);

class ResourceCreator {
public:
	virtual ~ResourceCreator() {}

	virtual const char* getFilePattern() const = 0;
	virtual void createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem) = 0;
	virtual void createResourceFromMemory(ResourceHandler& handler, void *data, size_t dataSize) = 0;

	virtual ResourceCreatorType getType() const = 0;
};

class DefaultResourceCreator final : public ResourceCreator {
public:
	virtual const char* getFilePattern() const { return "*"; }
	virtual void createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem);
	virtual void createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize);

	virtual ResourceCreatorType getType() const { return DefaultResourceCreatorType; }
};