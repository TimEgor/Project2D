#pragma once

#include <EntityManager/Entity.h>

#include <atomic>
#include <cstdint>

typedef uint32_t EntityID;

class EntityManager;

class EntityHandler final {
private:
	Entity& entity;
	EntityManager& manager;
	size_t entityAllocatorIndex;
	EntityID id;

public:
	EntityHandler(EntityID id, Entity& entity, EntityManager& manager, size_t entityAllocatorIndex)
		: entity(entity), manager(manager), entityAllocatorIndex(entityAllocatorIndex), id(id) {
		entity.handler = this;
	}
	EntityHandler(const EntityHandler&) = delete;
	EntityHandler(EntityHandler&& handler) = delete;

	Entity& getEntity() { return entity; }
	EntityID getID() { return id; }

	EntityManager& getManager() { return manager; }

	size_t getEntityAllocatorIndex() { return entityAllocatorIndex; }
};