#pragma once

#include <ResourceManager/ResourceHandler.h>

class ResourceReference final : public AtomicCounterObjetcBaseReference<ResourceHandler> {
	typedef AtomicCounterObjetcBaseReference<ResourceHandler> ReferenceBaseType;

public:
	ResourceReference() = default;
	ResourceReference(ResourceHandler* handler) : ReferenceBaseType(handler) {}
	ResourceReference(const ResourceReference& reference) : ReferenceBaseType(reference.object) {}

	ResourceReference& operator=(const ResourceReference& reference);

	bool isResourceReady() const { return ((ResourceHandler*)(object))->getResource(); }

	ResourceID getResourceID() const { return ((ResourceHandler*)(object))->getResourceID(); }

	template <typename T>
	T* getResource() {
		return (T*)(object);
	}

	template <typename T>
	const T* getResource() const {
		return (T*)(object);
	}
};
