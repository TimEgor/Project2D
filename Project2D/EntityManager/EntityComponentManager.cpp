#include "EntityComponentManager.h"

#include <MemoryManager/MemoryManager.h>
#include <EntityManager/Entity.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <EntityManager/EntityComponents/CppGameLogicEntityComponent.h>
#include <EntityManager/EntityComponents/Canvas/CanvasSpriteRendererEntityComponent.h>
#include <EntityManager/EntityComponents/Canvas/CanvasLabelEntityComponent.h>
#include <EntityManager/EntityComponentReference.h>

#include <cassert>

ComponentAllocators::AllocationInfo EntityComponentManager::allocateComponent(EntityComponentType type) {
    ComponentAllocators::AllocationInfo info;
    
    auto findIter = componentAllocators.find(type);
    assert(findIter != componentAllocators.end());
    if (findIter != componentAllocators.end()) {
        ComponentAllocators& allocatorVec = findIter->second;

        size_t oldSize = allocatorVec.size();
        info = allocatorVec.allocate();
        size_t newSize = allocatorVec.size();

        if (oldSize < newSize) {
            components.reserve(components.bucket_count() + ENTITIES_COMPONENTS_ALLOCATOR_SIZE * (newSize - oldSize));
        }
    }

    return info;
}

EntityComponentReferenceHandler* EntityComponentManager::createReference(EntityComponentHandler* componentHandler) {
    ReferenceAllocators::AllocationInfo info = referenceAllocators.allocate();

    EntityComponentReferenceHandler* referenceHandler = new (info.allocationAddress) EntityComponentReferenceHandler(componentHandler, level, info.allocatorID);

    return referenceHandler;
}

void EntityComponentManager::releaseReference(EntityComponentReferenceHandler* referenceHandler) {
    referenceAllocators.deallocate(referenceHandler->getAllocatorIndex(), referenceHandler);
}

bool EntityComponentManager::init(Level* currentLevel) {
    componentAllocators[SpriteRendererEntityComponentType].init(sizeof(SpriteRendererEntityComponent), ENTITIES_COMPONENTS_ALLOCATOR_SIZE);
    componentAllocators[CanvasSpriteRendererEntityComponentType].init(sizeof(CanvasSpriteRendererEntityComponent), ENTITIES_COMPONENTS_ALLOCATOR_SIZE);
    componentAllocators[CanvasLabelEntityComponentType].init(sizeof(CanvasLabelEntityComponent), ENTITIES_COMPONENTS_ALLOCATOR_SIZE);
    componentAllocators[CppGameLogicEntityComponentType].init(sizeof(CppGameLogicEntityComponent), ENTITIES_COMPONENTS_ALLOCATOR_SIZE);

    referenceAllocators.init(sizeof(EntityComponentReferenceHandler), 512);

    level = currentLevel;

    return true;
}

void EntityComponentManager::release() {
    components = std::unordered_map<EntityComponentID, EntityComponentHandler>();
    for (auto& typeAllocators : componentAllocators) {
        typeAllocators.second.release();
    }
}

void EntityComponentManager::deleteEntityComponent(EntityComponentID id) {
    auto findIter = components.find(id);
    if (findIter != components.end()) {
        EntityComponentHandler& handler = findIter->second;
        EntityComponent* component = handler.getComponent();

        EntityComponentReferenceHandler* referenceHandler = handler.getReferenceHandler();
        if (referenceHandler) {
            referenceHandler->setComponentHandler(nullptr);
        }

        ComponentAllocators& allocatorVec = componentAllocators[component->getEntityComponentType()];
        size_t allocatorIndex = allocatorVec.getAllocatorIndexByID(handler.getComponentAllocatorID());

        ArrayPoolAllocator& allocator = allocatorVec[allocatorIndex];
        component->release();
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

ComponentAllocators* EntityComponentManager::getEntityComponents(EntityComponentType type) {
    auto allocatorTypeIter = componentAllocators.find(type);
    if (allocatorTypeIter != componentAllocators.end()) {
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
