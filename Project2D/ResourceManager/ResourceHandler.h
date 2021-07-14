#pragma once

#include <ResourceManager/Resource.h>
#include <Multithreading/AtomicCounterObjectBase.h>

class ResourceHandler final : public AtomicCounterObjectBase {
private:
	std::atomic<Resource*> resource;
	ResourceID id;

	virtual void selfDestroy() override;

public:
	ResourceHandler(ResourceID id) : resource(nullptr), id(id) {}
	ResourceHandler(const ResourceHandler&) = delete;
	ResourceHandler(ResourceHandler&& handler) = delete;

	Resource* getResource() { return resource.load(); }
	void setResource(Resource* newResource) { resource = newResource; }

	ResourceID getResourceID() { return id; }
};