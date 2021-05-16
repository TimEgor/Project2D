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

	bool isNull() const { return handler == nullptr; }
	bool isResourceReady() const { return handler->getResource(); }

	ResourceID getResourceID() const { return handler->getResourceID(); }

	template <typename T>
	T& getResource();

	template <typename T>
	const T& getResource() const;
};

template<typename T>
inline T& ResourceReference::getResource() {
	return *(T*)(handler->getResource());
}

template<typename T>
inline const T& ResourceReference::getResource() const {
	return *(T*)(handler->getResource());
}
