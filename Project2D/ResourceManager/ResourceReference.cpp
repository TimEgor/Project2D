#include "ResourceReference.h"

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
	}
}

ResourceReference& ResourceReference::operator=(const ResourceReference& reference) {
	if (reference.handler) {
		reference.handler->incrementRefCounter();
	}
	
	if (handler) {
		handler->decrementRefCounter();
	}
	
	handler = reference.handler;

	return *this;
}
