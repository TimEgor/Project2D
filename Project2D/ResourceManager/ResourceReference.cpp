#include "ResourceReference.h"
#include <ResourceManager/ResourceManager.h>

void ResourceReference::unloadResource() {
	if (handler->getRefCounter() == 0) {
		ResourceManager::get().unloadResource(*handler);
	}
}

ResourceReference::ResourceReference(ResourceHandler* handler) : handler(handler) {
	if (handler) {
		handler->incrementRefCounter();
	}
}

ResourceReference::ResourceReference(const ResourceReference& reference) : handler(reference.handler) {
	if (handler) {
		handler->incrementRefCounter();
	}
}

ResourceReference::~ResourceReference() {
	if (handler) {
		handler->decrementRefCounter();
		unloadResource();
	}
}

ResourceReference& ResourceReference::operator=(const ResourceReference& reference) {
	if (reference.handler) {
		reference.handler->incrementRefCounter();
	}
	
	if (handler) {
		handler->decrementRefCounter();
		unloadResource();
	}
	
	handler = reference.handler;

	return *this;
}
