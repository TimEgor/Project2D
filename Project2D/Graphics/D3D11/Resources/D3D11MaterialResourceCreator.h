#pragma once

#include <ResourceManager/ResourceCreator.h>

class D3D11MaterialResourceCreator final : public ResourceCreator {
public:
	virtual const char* getFilePattern() const { return "*.material"; }
	virtual void createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem);
	virtual void createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize);
};