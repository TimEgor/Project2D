#include <ResourceManager/ResourceCreator.h>

class VertexShaderResourceCreator final : public ResourceCreator {
public:
	virtual const char* getFilePattern() const { return "*.vshader"; }
	virtual void createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem);
	virtual void createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize);

	virtual ResourceCreatorType getType() const { return VertexShaderResourceCreatorType; }
};

class VertexShaderResourceCreator final : public ResourceCreator {
public:
	virtual const char* getFilePattern() const { return "*.pshader"; }
	virtual void createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem);
	virtual void createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize);

	virtual ResourceCreatorType getType() const { return PixelShaderResourceCreatorType; }
};