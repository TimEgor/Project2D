#pragma once

#include <ResourceManager/Resource.h>

class ResourceName;

class FileSystem {
public:
	FileSystem() {}
	virtual ~FileSystem() {}

	virtual bool init(const ResourceName &path) = 0;
	virtual void release() = 0;

	virtual bool getResourceSize(const ResourceName& resourceName, size_t& resourceSize) = 0;
	virtual bool getResourceData(const ResourceName& resourceName, void *data, size_t resourceSize) = 0;
	virtual bool getResourceBinaryData(const ResourceName& resourceName, void *data, size_t resourceSize) = 0;
};

class IndexedFileSystem : public FileSystem {
public:
	IndexedFileSystem() {}

	virtual bool getResourceSize(ResourceID resourceID, size_t& resourceSize) = 0;
	virtual bool getResourceData(ResourceID resourceID, void* data, size_t resourceSize) = 0;
	virtual bool getResourceBinaryData(ResourceID resourceID, void* data, size_t resourceSize) = 0;
};