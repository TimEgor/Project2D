#include "EntityHandler.h"

#include <EntityManager/Entity.h>

EntityHandler::EntityHandler(EntityHandler&& handler)
	: entity(handler.entity), entityAllocatorIndex(handler.entityAllocatorIndex), selfAllocatorIndex(handler.selfAllocatorIndex),
	refCounter(handler.refCounter.load()), id(handler.id) {
	entity->handler = this;

	handler.entity = nullptr;
	handler.refCounter = 0;
	handler.entityAllocatorIndex = 0;
	handler.selfAllocatorIndex = 0;
	handler.id = 0;
}
