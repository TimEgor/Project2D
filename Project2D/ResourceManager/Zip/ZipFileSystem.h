#pragma once

#include <ResourceManager/FileSystem.h>
#include <ResourceManager/Zip/ZipFile.h>

class ZipFileSystem final : public IndexedFileSystem {
private:
	ZipFile zipFile;

public:
	ZipFileSystem() = default;
	~ZipFileSystem() { release(); }

	virtual bool init(const ResourceName& path) override;
	virtual void release() override;

	virtual bool getResourceSize(const ResourceName& resourceName, size_t& resourceSize) override;
	virtual bool getResourceSize(ResourceID resourceID, size_t& resourceSize) override;
	virtual bool getResourceData(const ResourceName& resourceName, void* data, size_t resourceSize) override;
	virtual bool getResourceData(ResourceID resourceID, void* data, size_t resourceSize) override;
	virtual bool getResourceBinaryData(const ResourceName& resourceName, void* data, size_t resourceSize) override;
	virtual bool getResourceBinaryData(ResourceID resourceID, void* data, size_t resourceSize) override;

	ResourceName getResourceName(ResourceID resourceID);

};