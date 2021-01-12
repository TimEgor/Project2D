#pragma once

#include <EntityManager/EntityComponentHandler.h>
#include <MemoryManager/Allocators/LinkingPoolAllocatorVector.h>

#include <unordered_map>
#include <vector>

#define ENTITIES_COMPONENTS_ALLOCATOR_SIZE 4096

class EntityComponentManager final {
private:
	std::unordered_map<EntityComponentID, EntityComponentHandler> components;
	std::unordered_map<EntityComponentType, LinkingPoolAllocatorVector> allocators;

	EntityComponentID nextEntityComponentID;

	LinkingPoolAllocatorVector::AllocationInfo allocateComponent(EntityComponentType type);

public:
	EntityComponentManager() : nextEntityComponentID(1) {}
	~EntityComponentManager() { release(); }

	bool init();
	void release();

	void deleteEntityComponent(EntityComponentID id);
	void deleteEntityComponent(EntityComponent* component);

	EntityComponent* getEntityComponent(EntityComponentID id);
};