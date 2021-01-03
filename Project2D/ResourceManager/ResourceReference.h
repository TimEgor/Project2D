#pragma once

#include <ResourceManager/ResourceHandler.h>

class ResourceReference final {
private:
	ResourceHandler& handler;

public:
	ResourceReference(ResourceHandler& handler);
	ResourceReference(const ResourceReference& reference);
	~ResourceReference();

	ResourceReference& operator=(const ResourceReference&) = delete;

	template <typename T>
	T& getResource();
};

template<typename T>
inline T& ResourceReference::getResource() {
	return *(T*)(handler.getResource());
}
