#pragma once

#include <EntityManager/EntityComponentHandler.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>
#include <MemoryManager/Allocators/ArrayPoolAllocator.h>
#include <MemoryManager/Allocators/PoolAllocator.h>

#include <unordered_map>
#include <vector>

#define ENTITIES_COMPONENTS_ALLOCATOR_SIZE 4096

class Level;
class SpriteRendererEntityComponent;
class CanvasSpriteRendererEntityComponent;
class EntityComponentReferenceHandler;
class EntityComponentReference;

class EntityComponentManager final {
	friend EntityComponentReference;
	friend EntityComponentHandler;

	typedef PoolAllocatorVector<ArrayPoolAllocator> ComponentAllocators;
	typedef PoolAllocatorVector<PoolAllocator> ReferenceAllocators;

private:
	std::unordered_map<EntityComponentID, EntityComponentHandler> components;
	std::unordered_map<EntityComponentType, ComponentAllocators> componentAllocators;
	std::unordered_map<EntityComponentType, size_t> counters;
	ReferenceAllocators referenceAllocators;

	Level* level = nullptr;

	EntityComponentID nextEntityComponentID = 1;

	ComponentAllocators::AllocationInfo allocateComponent(EntityComponentType type);

	EntityComponentReferenceHandler* createReference(EntityComponentHandler* componentHandler);
	void releaseReference(EntityComponentReferenceHandler* referenceHandler);

public:
	EntityComponentManager() = default;
	~EntityComponentManager() { release(); }

	bool init(Level* level);
	void release();

	template <typename ComponentType>
	ComponentType* createComponent(EntityComponentType type);

	void deleteEntityComponent(EntityComponentID id);
	void deleteEntityComponent(EntityComponent* component);

	EntityComponent* getEntityComponent(EntityComponentID id);
	ComponentAllocators* getEntityComponents(EntityComponentType type);

	size_t getEntityComponentsNum(EntityComponentType type) const;
};
