#pragma once

#include <ResourceManager/ResourceCreator.h>

class PNG_TextureResourceCreator final : public ResourceCreator {
public:
	virtual const char* getFilePattern() const { return "*.png"; }
	virtual void createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem);
	virtual void createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize);

	virtual ResourceCreatorType getType() const { return PNG_TextureResourceCreatorType; }
};