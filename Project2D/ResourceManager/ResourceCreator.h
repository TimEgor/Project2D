#pragma once

#include <ResourceManager/Resource.h>

class ResourceHandler;
class ResourceName;
class ZipFileSystem;
class NativeFileSystem;

class ResourceCreator {
public:
	virtual ~ResourceCreator() {}

	virtual const char* getFilePattern() const = 0;
	virtual void createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem) = 0;
	virtual void createResourceFromMemory(ResourceHandler& handler, void *data, size_t dataSize) = 0;
};

class DefaultResourceCreator final : public ResourceCreator {
public:
	virtual const char* getFilePattern() const { return "*"; }
	virtual void createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem);
	virtual void createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize);
};