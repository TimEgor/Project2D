#pragma once

#include <ResourceManager/ResourceCreator.h>

class D3D11PixelShaderResourceCreator final : public ResourceCreator {
public:
	virtual const char* getFilePattern() const { return "*.pshader"; }
	virtual void createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem);
	virtual void createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize);
};

class D3D11VertexShaderResourceCreator final : public ResourceCreator {
public:
	virtual const char* getFilePattern() const { return "*.vshader"; }
	virtual void createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem);
	virtual void createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize);
};

class D3D11BinaryPixelShaderResourceCreator final : public ResourceCreator {
public:
	virtual const char* getFilePattern() const { return "*.bpshader"; }
	virtual void createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem);
	virtual void createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize);
};

class D3D11BinaryVertexShaderResourceCreator final : public ResourceCreator {
public:
	virtual const char* getFilePattern() const { return "*.bvshader"; }
	virtual void createResourceFromZipFileSystem(ResourceHandler& handler, ResourceID resourceID, ZipFileSystem& zipFileSystem);
	virtual void createResourceFromMemory(ResourceHandler& handler, void* data, size_t dataSize);
};