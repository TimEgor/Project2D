#include "EntityComponentManager.h"

#include <MemoryManager/MemoryManager.h>
#include <EntityManager/Entity.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>

#include <cassert>

EntityComponentManager::Allocators::AllocationInfo EntityComponentManager::allocateComponent(EntityComponentType type) {
    Allocators& allocatorVec = allocators.at(type);
    size_t oldSize = allocatorVec.size();
    Allocators::AllocationInfo info = allocatorVec.allocate();
    size_t newSize = allocatorVec.size();

    if (oldSize < newSize) {
        components.reserve(components.bucket_count() + ENTITIES_COMPONENTS_ALLOCATOR_SIZE * (newSize - oldSize));
    }

    return info;
}

bool EntityComponentManager::init(Level* level) {
    Heap* defaultHeap = MemoryManager::get().getDefaultHeap();

    allocators[SpriteRendererEntityComponentType].init(defaultHeap, sizeof(SpriteRendererEntityComponent), ENTITIES_COMPONENTS_ALLOCATOR_SIZE);

    return true;
}

void EntityComponentManager::release() {
    components = std::unordered_map<EntityComponentID, EntityComponentHandler>();
    for (auto& typeAllocators : allocators) {
        typeAllocators.second.release();
    }
}

SpriteRendererEntityComponent* EntityComponentManager::createSpriteRendererEntityComponent() {
    Allocators::AllocationInfo allocationInfo = allocateComponent(SpriteRendererEntityComponentType);
    SpriteRendererEntityComponent* newComponent = new (allocationInfo.allocationAddress) SpriteRendererEntityComponent();

    components.emplace(std::piecewise_construct, std::forward_as_tuple(nextEntityComponentID),
        std::forward_as_tuple(nextEntityComponentID, newComponent, allocationInfo.allocatorID, level));

    ++nextEntityComponentID;
    ++counters[SpriteRendererEntityComponentType];

    return newComponent;
}

void EntityComponentManager::deleteEntityComponent(EntityComponentID id) {
    auto findIter = components.find(id);
    if (findIter != components.end()) {
        EntityComponentHandler& handler = findIter->second;
        EntityComponent* component = handler.getComponent();

        Allocators& allocatorVec = allocators[component->getEntityComponentType()];
        size_t allocatorIndex = allocatorVec.getAllocatorIndexByID(handler.getComponentAllocatorID());

        ArrayPoolAllocator& allocator = allocatorVec[allocatorIndex];
        allocator.deallocate(component);
        if (allocator.size()) {
            EntityComponentHandler* reallocatingComponentHandler = component->getHandler();
            if (reallocatingComponentHandler) {
                reallocatingComponentHandler->setComponent(component);
            }
        }

        components.erase(findIter);
    }
}

void EntityComponentManager::deleteEntityComponent(EntityComponent* component) {
    deleteEntityComponent(component->getID());
}

EntityComponent* EntityComponentManager::getEntityComponent(EntityComponentID id) {
    auto findIter = components.find(id);
    if (findIter == components.end()) {
        return nullptr;
    }

    return findIter->second.getComponent();
}

const EntityComponentManager::Allocators* EntityComponentManager::getEntityComponents(EntityComponentType type) const {
    auto allocatorTypeIter = allocators.find(type);
    if (allocatorTypeIter != allocators.end()) {
        return &allocatorTypeIter->second;
    }

    return nullptr;
}

size_t EntityComponentManager::getEntityComponentsNum(EntityComponentType type) const {
    auto counterTypeIter = counters.find(type);
    if (counterTypeIter != counters.end()) {
        return counterTypeIter->second;
    }

    return 0;
}
