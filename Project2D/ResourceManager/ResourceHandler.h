#pragma once

#include <atomic>

class Resource;

class ResourceHandler final {
private:
	std::atomic<Resource*> resource;
	std::atomic_size_t refCounter;

public:
	ResourceHandler() : resource(nullptr), refCounter(0) {}
	ResourceHandler(const ResourceHandler&) = delete;
	ResourceHandler(ResourceHandler&& handler) : resource(handler.resource.load()), refCounter(handler.refCounter.load()) {
		handler.resource = nullptr;
		handler.refCounter = 0;
	}

	Resource* getResource() { return resource.load(); }
	void setResource(Resource* newResource) { resource = newResource; }

	size_t getRefCounter() { return refCounter; }
	void incrementRefCounter() { ++refCounter; }
	void decrementRefCounter() { --refCounter; }
};