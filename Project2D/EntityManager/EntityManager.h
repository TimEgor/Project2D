#pragma once

#include <EntityManager/EntityHandler.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>
#include <MemoryManager/Allocators/PoolAllocator.h>

#include <unordered_map>

#define ENTITIES_ALLOCATOR_SIZE 4096

class Level;

class EntityManager final {
	typedef PoolAllocatorVector<PoolAllocator> Allocators;

private:
	std::unordered_map<EntityID, EntityHandler> entities;
	Allocators allocators;

	Level* level;

	EntityID nextEntityID;

public:
	EntityManager() : nextEntityID(1) {}
	~EntityManager() { release(); }

	bool init(Level* level);
	void release();

	Entity* createEntity();
	void deleteEntity(EntityID id);
	void deleteEntity(Entity* entity);

	Entity* getEntity(EntityID id);
};