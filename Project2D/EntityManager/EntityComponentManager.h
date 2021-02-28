#pragma once

#include <EntityManager/EntityComponentHandler.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>
#include <MemoryManager/Allocators/ArrayPoolAllocator.h>

#include <unordered_map>
#include <vector>

#define ENTITIES_COMPONENTS_ALLOCATOR_SIZE 4096

class Level;
class SpriteRendererEntityComponent;

class EntityComponentManager final {
	typedef PoolAllocatorVector<ArrayPoolAllocator> Allocators;

private:
	std::unordered_map<EntityComponentID, EntityComponentHandler> components;
	std::unordered_map<EntityComponentType, Allocators> allocators;

	Level* level;

	EntityComponentID nextEntityComponentID;

	Allocators::AllocationInfo allocateComponent(EntityComponentType type);

public:
	EntityComponentManager() : nextEntityComponentID(1) {}
	~EntityComponentManager() { release(); }

	bool init(Level* level);
	void release();

	SpriteRendererEntityComponent* createSpriteRendererEntityComponent();

	void deleteEntityComponent(EntityComponentID id);
	void deleteEntityComponent(EntityComponent* component);

	EntityComponent* getEntityComponent(EntityComponentID id);
};