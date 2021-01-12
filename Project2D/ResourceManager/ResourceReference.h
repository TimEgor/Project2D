#pragma once

#include <ResourceManager/ResourceHandler.h>

class ResourceReference final {
private:
	ResourceHandler* handler;

public:
	ResourceReference() : handler(nullptr) {}
	ResourceReference(ResourceHandler* handler);
	ResourceReference(const ResourceReference& reference);
	~ResourceReference();

	ResourceReference& operator=(const ResourceReference& reference);

	bool isValid() const { return handler; }

	template <typename T>
	T& getResource();
};

template<typename T>
inline T& ResourceReference::getResource() {
	return *(T*)(handler->getResource());
}
