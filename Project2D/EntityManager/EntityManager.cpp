#include "EntityManager.h"

#include <MemoryManager/MemoryManager.h>
#include <EntityManager/EntityHandler.h>

bool EntityManager::init() {
    entities.reserve(ENTITIES_ALLOCATOR_SIZE);
    return allocators.init(MemoryManager::get().getDefaultHeap(), sizeof(Entity), ENTITIES_ALLOCATOR_SIZE);
}

void EntityManager::release() {
    entities = std::unordered_map<EntityID, EntityHandler>();
    allocators.release();
}

Entity* EntityManager::createEntity() {
    size_t oldAllocatorCount = allocators.size();

    PoolAllocatorVector::AllocationInfo allocationInfo = allocators.allocate();
    Entity* newEntity = new (allocationInfo.allocationAddress) Entity();

    size_t newAllocatorCount = allocators.size();
    if (oldAllocatorCount < newAllocatorCount) {
        entities.reserve(newAllocatorCount * ENTITIES_ALLOCATOR_SIZE);
    }

    entities.emplace(std::piecewise_construct, std::forward_as_tuple(nextEntityID), std::forward_as_tuple(nextEntityID, *newEntity, *this, allocationInfo.allocatorIndex));

    ++nextEntityID;

    return newEntity;
}

void EntityManager::deleteEntity(EntityID id) {
    auto findIter = entities.find(id);
    if (findIter != entities.end()) {
        EntityHandler& handler = findIter->second;
        Entity* entity = &handler.getEntity();
        entity->removeAllComponents();
        allocators.deallocate(handler.getEntityAllocatorIndex(), entity);

        entities.erase(findIter);
    }
}

void EntityManager::deleteEntity(Entity* entity) {
    auto findIter = entities.find(entity->getID());
    if (findIter != entities.end()) {
        EntityHandler& handler = findIter->second;
        entity->removeAllComponents();
        allocators.deallocate(handler.getEntityAllocatorIndex(), entity);

        entities.erase(findIter);
    }
}

Entity* EntityManager::getEntity(EntityID id) {
    auto findIter = entities.find(id);
    if (findIter == entities.end()) {
        return nullptr;
    }

    return &(findIter->second.getEntity());
}
