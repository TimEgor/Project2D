#pragma once

#include <EntityManager/Entity.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>

#include <atomic>
#include <cstdint>

typedef uint32_t EntityID;

class Level;

class EntityHandler final {
private:
	Entity* entity;
	Level* level;
	AllocatorID allocatorID;
	EntityID id;

public:
	EntityHandler(EntityID id, Entity* entity, size_t entityAllocatorID, Level* level)
		: entity(entity), level(level), allocatorID(entityAllocatorID), id(id) {
		entity->handler = this;
	}
	EntityHandler(const EntityHandler&) = delete;
	EntityHandler(EntityHandler&&) = delete;

	Entity* getEntity() { return entity; }
	EntityID getID() { return id; }

	size_t getEntityAllocatorID() { return allocatorID; }
};