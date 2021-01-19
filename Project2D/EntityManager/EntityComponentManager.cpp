#include "EntityComponentManager.h"

#include <MemoryManager/MemoryManager.h>

LinkingPoolAllocatorVector::AllocationInfo EntityComponentManager::allocateComponent(EntityComponentType type) {
    LinkingPoolAllocatorVector& allocatorVec = allocators.at(type);
    size_t oldSize = allocatorVec.size();
    LinkingPoolAllocatorVector::AllocationInfo info = allocatorVec.allocate();
    size_t newSize = allocatorVec.size();

    if (oldSize < newSize) {
        components.reserve(components.bucket_count() + ENTITIES_COMPONENTS_ALLOCATOR_SIZE * (newSize - oldSize));
    }

    components.emplace(std::piecewise_construct, std::forward_as_tuple(nextEntityComponentID),
        std::forward_as_tuple(nextEntityComponentID, *((EntityComponent*)(info.allocationAddress)), info.allocatorIndex));

    ++nextEntityComponentID;

    return info;
}

bool EntityComponentManager::init() {
    return true;
}

void EntityComponentManager::release() {
    components = std::unordered_map<EntityComponentID, EntityComponentHandler>();
    for (auto& typeAllocators : allocators) {
        typeAllocators.second.release();
    }
}

void EntityComponentManager::deleteEntityComponent(EntityComponentID id) {
    auto findIter = components.find(id);
    if (findIter != components.end()) {
        EntityComponentHandler& handler = findIter->second;
        EntityComponent* component = &handler.getComponent();
        allocators[component->getEntityComponentType()].deallocate(handler.getComponentAllocatorIndex(), component);

        components.erase(findIter);
    }
}

void EntityComponentManager::deleteEntityComponent(EntityComponent* component) {
    auto findIter = components.find(component->getID());
    if (findIter != components.end()) {
        EntityComponentHandler& handler = findIter->second;
        allocators[component->getEntityComponentType()].deallocate(handler.getComponentAllocatorIndex(), component);

        components.erase(findIter);
    }
}

EntityComponent* EntityComponentManager::getEntityComponent(EntityComponentID id) {
    auto findIter = components.find(id);
    if (findIter == components.end()) {
        return nullptr;
    }

    return &(findIter->second.getComponent());
}
