#pragma once

#include <EntityManager/EntityHandler.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>
#include <MemoryManager/Allocators/PoolAllocator.h>

#include <unordered_map>

#define ENTITIES_ALLOCATOR_SIZE 4096

class BaseGameSpace;

class EntityManager final {
	typedef PoolAllocatorVector<PoolAllocator> Allocators;

private:
	std::unordered_map<EntityID, EntityHandler> entities;
	Allocators allocators;

	BaseGameSpace* level;

	EntityID nextEntityID;

public:
	EntityManager() : nextEntityID(1) {}
	~EntityManager() { release(); }

	bool init(BaseGameSpace* level);
	void release();

	Entity* createEntity();
	void deleteEntity(EntityID id);
	void deleteEntity(Entity* entity);

	Entity* getEntity(EntityID id);
};