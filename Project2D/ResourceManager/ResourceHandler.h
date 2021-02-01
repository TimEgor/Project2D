#pragma once

#include <ResourceManager/Resource.h>

#include <atomic>

class ResourceHandler final {
private:
	std::atomic<Resource*> resource;
	std::atomic_size_t refCounter;
	ResourceID id;

public:
	ResourceHandler(ResourceID id) : resource(nullptr), refCounter(0), id(id) {}
	ResourceHandler(const ResourceHandler&) = delete;
	ResourceHandler(ResourceHandler&& handler) = delete;

	Resource* getResource() { return resource.load(); }
	void setResource(Resource* newResource) { resource = newResource; }

	ResourceID getResourceID() { return id; }

	size_t getRefCounter() { return refCounter; }
	void incrementRefCounter() { ++refCounter; }
	void decrementRefCounter() { --refCounter; }
};