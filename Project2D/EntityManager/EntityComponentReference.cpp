#include "EntityComponentReference.h"

#include <BaseGameLogic/Level.h>
#include <EntityManager/EntityComponentManager.h>

void EntityComponentReference::releaseReference() {
	if (handler->getRefCounter() == 0) {
		handler->getLevel()->getEntityComponentManager()->releaseReference(handler);
	}
}

EntityComponentReference::EntityComponentReference(EntityComponentReferenceHandler* handler) : handler(handler) {
	if (handler) {
		handler->incrementRefCounter();
	}
}

EntityComponentReference::EntityComponentReference(const EntityComponentReference& reference) : handler(reference.handler) {
	if (handler) {
		handler->incrementRefCounter();
	}
}

EntityComponentReference::~EntityComponentReference() {
	if (handler) {
		handler->decrementRefCounter();
		releaseReference();
	}
}

EntityComponentReference& EntityComponentReference::operator=(const EntityComponentReference& reference) {
	if (reference.handler) {
		reference.handler->incrementRefCounter();
	}

	if (handler) {
		handler->decrementRefCounter();
		releaseReference();
	}

	handler = reference.handler;

	return *this;
}
