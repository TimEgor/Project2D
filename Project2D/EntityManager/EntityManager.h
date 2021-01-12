#pragma once

#include <EntityManager/EntityHandler.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>

#include <unordered_map>
#include <vector>

#define ENTITIES_ALLOCATOR_SIZE 4096

class EntityManager final {
private:
	std::unordered_map<EntityID, EntityHandler> entities;
	PoolAllocatorVector allocators;

	EntityID nextEntityID;

public:
	EntityManager() : nextEntityID(1) {}
	~EntityManager() { release(); }

	bool init();
	void release();

	Entity* createEntity();
	void deleteEntity(EntityID id);
	void deleteEntity(Entity* entity);

	Entity* getEntity(EntityID id);
};