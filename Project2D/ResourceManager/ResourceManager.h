#pragma once

#include <ResourceManager/ResourceName.h>
#include <ResourceManager/ResourceHandler.h>
#include <ResourceManager/ResourceReference.h>

#include <unordered_map>
#include <mutex>

class ResourceAsyncLoader;
class ResourceCreator;
class DefaultResourceCreator;
class ZipFileSystem;
class NativeFileSystem;

class ResourceManager final {
	friend ResourceAsyncLoader;
	friend ResourceReference;

private:
	std::unordered_map<ResourceID, ResourceHandler> resources;
	std::unordered_map<ResourceType, ResourceCreator*> resourceCreators;
	std::mutex resMutex;

	ResourceAsyncLoader* resourceAsyncLoader;

	ZipFileSystem* zipFileSystem;
	NativeFileSystem* nativeFileSystem;

	DefaultResourceCreator* defaultResourceCreator;

	ResourceManager() = default;

	bool initResourceCreators();
	ResourceCreator& chooseResourceCreator(const ResourceName &resourceName);

	ResourceHandler& createNewHandler(ResourceID resourceID);

	ResourceReference loadNewResourceFromArchive(ResourceHandler& handler, const ResourceName& resourceName, ResourceID resourceID);

	void unloadResource(ResourceHandler& handler);

public:
	static ResourceManager& get();

	bool init();
	void release();

	ResourceReference getResourceFromArchive(const ResourceName& resourceName);
	ResourceReference getResourceFromArchive(ResourceID resourceID);
	ResourceReference getResourceAsyncFromArchive(const ResourceName& resourceName);
	ResourceReference getResourceAsyncFromArchive(ResourceID resourceID);

	void* getRawResourceFromArchive(const ResourceName& resourceName, size_t& resourceSize);
	void* getRawResourceFromNativeFileSystem(const ResourceName& resourceName, size_t& resourceSize);
};