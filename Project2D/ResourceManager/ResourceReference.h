#pragma once

#include <ResourceManager/ResourceHandler.h>

class ResourceReference final {
private:
	ResourceHandler* handler;

	void unloadResource();

public:
	ResourceReference() : handler(nullptr) {}
	ResourceReference(ResourceHandler* handler);
	ResourceReference(const ResourceReference& reference);
	~ResourceReference();

	ResourceReference& operator=(const ResourceReference& reference);

	bool isValid() const { return handler; }
	bool isResourceReady() const { return handler->getResource(); }

	template <typename T>
	T& getResource();
};

template<typename T>
inline T& ResourceReference::getResource() {
	return *(T*)(handler->getResource());
}
