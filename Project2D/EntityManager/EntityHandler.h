#pragma once

#include <EntityManager/Entity.h>

#include <atomic>
#include <cstdint>

typedef uint32_t EntityID;

class EntityManager;

class EntityHandler final {
private:
	Entity& entity;
	size_t entityAllocatorIndex;
	EntityID id;

public:
	EntityHandler(EntityID id, Entity& entity, size_t entityAllocatorIndex)
		: entity(entity), entityAllocatorIndex(entityAllocatorIndex), id(id) {
		entity.handler = this;
	}
	EntityHandler(const EntityHandler&) = delete;
	EntityHandler(EntityHandler&&) = delete;

	Entity& getEntity() { return entity; }
	EntityID getID() { return id; }

	size_t getEntityAllocatorIndex() { return entityAllocatorIndex; }
};