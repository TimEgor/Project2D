#pragma once

#include <EntityManager/EntityComponentHandler.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>
#include <MemoryManager/Allocators/ArrayPoolAllocator.h>

#include <unordered_map>
#include <vector>

#define ENTITIES_COMPONENTS_ALLOCATOR_SIZE 4096

class BaseGameSpace;
class SpriteRendererEntityComponent;

class EntityComponentManager final {
	typedef PoolAllocatorVector<ArrayPoolAllocator> Allocators;

private:
	std::unordered_map<EntityComponentID, EntityComponentHandler> components;
	std::unordered_map<EntityComponentType, Allocators> allocators;
	std::unordered_map<EntityComponentType, size_t> counters;

	BaseGameSpace* level;

	EntityComponentID nextEntityComponentID;

	Allocators::AllocationInfo allocateComponent(EntityComponentType type);

public:
	EntityComponentManager() : nextEntityComponentID(1) {}
	~EntityComponentManager() { release(); }

	bool init(BaseGameSpace* level);
	void release();

	template <typename EntityType>
	EntityType* createEntityComponent();

	SpriteRendererEntityComponent* createSpriteRendererEntityComponent();

	void deleteEntityComponent(EntityComponentID id);
	void deleteEntityComponent(EntityComponent* component);

	EntityComponent* getEntityComponent(EntityComponentID id);
	const Allocators* getEntityComponents(EntityComponentType type) const;

	size_t getEntityComponentsNum(EntityComponentType type) const;
};

template<typename EntityType>
inline EntityType* EntityComponentManager::createEntityComponent() {
	return nullptr;
}

template <>
inline SpriteRendererEntityComponent* EntityComponentManager::createEntityComponent<SpriteRendererEntityComponent>() {
	return createSpriteRendererEntityComponent();
}
