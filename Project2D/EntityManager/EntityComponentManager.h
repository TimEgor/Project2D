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

typedef PoolAllocatorVector<ArrayPoolAllocator> ComponentAllocators;
typedef PoolAllocatorVector<PoolAllocator> ReferenceAllocators;

class EntityComponentManager final {
	friend EntityComponentReference;
	friend EntityComponentHandler;

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

	template <typename ComponentType, typename ...Args>
	ComponentType* createComponent(Args... args);

	void deleteEntityComponent(EntityComponentID id);
	void deleteEntityComponent(EntityComponent* component);

	EntityComponent* getEntityComponent(EntityComponentID id);
	ComponentAllocators* getEntityComponents(EntityComponentType type);

	size_t getEntityComponentsNum(EntityComponentType type) const;
};

template<typename ComponentType, typename ...Args>
inline ComponentType* EntityComponentManager::createComponent(Args... args) {
	EntityComponentType type = ComponentType::getType();

	ComponentAllocators::AllocationInfo allocationInfo = allocateComponent(type);
	ComponentType* newComponent = nullptr;

	assert(allocationInfo.allocationAddress);
	if (allocationInfo.allocationAddress) {
		auto newHandlerIter = components.emplace(std::piecewise_construct, std::forward_as_tuple(nextEntityComponentID),
			std::forward_as_tuple(nextEntityComponentID, (ComponentType*)(allocationInfo.allocationAddress), allocationInfo.allocatorID, level));

		newComponent = new (allocationInfo.allocationAddress) ComponentType(&newHandlerIter.first->second, args...);

		++nextEntityComponentID;
		++counters[type];
	}

	return newComponent;
}
