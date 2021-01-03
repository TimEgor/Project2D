#pragma once

#include <ResourceManager/FileSystem.h>

#include <filesystem>

class NativeFileSystem final : public FileSystem {
private:
	bool loadRawDataBase(const ResourceName& resourceName, void* data, size_t size, std::ios_base::openmode mode);

public:
	NativeFileSystem() {}
	virtual ~NativeFileSystem() {}

	virtual bool init(const ResourceName& path) override { return true; }
	virtual void release() override {}

	virtual bool getResourceSize(const ResourceName& resourceName, size_t& resourceSize) override;
	virtual bool getResourceData(const ResourceName& resourceName, void* data, size_t resourceSize) override;
	virtual bool getResourceBinaryData(const ResourceName& resourceName, void* data, size_t resourceSize) override;
};