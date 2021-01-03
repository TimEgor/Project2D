#include "ResourceReference.h"

ResourceReference::ResourceReference(ResourceHandler& handler) : handler(handler) {
	handler.incrementRefCounter();
}

ResourceReference::ResourceReference(const ResourceReference& reference) : handler(reference.handler) {
	handler.incrementRefCounter();
}

ResourceReference::~ResourceReference() {
	handler.decrementRefCounter();
}