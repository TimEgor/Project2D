#include "BaseGameSpace.h"

#include <EntityManager/EntityManager.h>
#include <EntityManager/EntityComponentManager.h>
#include <MemoryManager/MemoryManager.h>
#include <Graphics/RenderingOrder.h>

bool BaseGameSpace::init() {
    entityManager = new EntityManager();
    entityComponentManager = new EntityComponentManager();

    if (!entityManager || !entityComponentManager) {
        release();
        return false;
    }

    if (!entityManager->init(this)) {
        release();
        return false;
    }

    if (!entityComponentManager->init(this)) {
        release();
        return false;
    }

    renderingOrder = new RenderingOrder();
    if (!renderingOrder->init(MemoryManager::get().getDefaultHeap())) {
        release();
        return false;
    }

    return true;
}

void BaseGameSpace::release() {
    if (entityManager) {
        entityManager->release();
        delete entityManager;
        entityManager = nullptr;
    }

    if (entityComponentManager) {
        entityComponentManager->release();
        delete entityComponentManager;
        entityComponentManager = nullptr;
    }

    if (renderingOrder) {
        renderingOrder->release();
        delete renderingOrder;
        renderingOrder = nullptr;
    }
}

EntityComponent* BaseGameSpace::createEntityComponent(EntityComponentType type, Entity* parent) {
    assert(parent);

    EntityComponent* component = nullptr;

    switch (type) {
    case SpriteRendererEntityComponentType:
        component = (EntityComponent*)(entityComponentManager->createSpriteRendererEntityComponent());
        break;
    default:
        break;
    }

    parent->addComponent(component->getID());
    component->getHandler()->setParentID(parent->getID());

    return component;
}

EntityComponent* BaseGameSpace::createEntityComponent(EntityComponentType type, EntityID parentID) {
    Entity* entity = entityManager->getEntity(parentID);
    return createEntityComponent(type, entity);
}

void BaseGameSpace::deleteEntityComponent(EntityComponent* component) {
    assert(component);

    Entity* entity = entityManager->getEntity(component->getParentID());
    entity->removeComponent(component->getID());

    entityComponentManager->deleteEntityComponent(component);
}

void BaseGameSpace::deleteEntityComponent(EntityComponentID id) {
    EntityComponent* component = entityComponentManager->getEntityComponent(id);
    deleteEntityComponent(component);
}

void BaseGameSpace::removeEntityComponentsFromEntity(Entity* entity) {
    deleteEntityComponentsFromEntity(entity);
    entity->removeAllComponents();
}

void BaseGameSpace::removeEntityComponentsFromEntity(EntityID id) {
    Entity* entity = entityManager->getEntity(id);
    removeEntityComponentsFromEntity(entity);
}

void BaseGameSpace::deleteEntityWithoutNode(Entity* entity) {
    deleteEntityComponentsFromEntity(entity);
    entityManager->deleteEntity(entity);
}

void BaseGameSpace::deleteEntityWithoutNode(EntityID id) {
    Entity* entity = entityManager->getEntity(id);
    deleteEntityWithoutNode(entity);
}

void BaseGameSpace::deleteEntityComponentsFromEntity(Entity* entity) {
    auto components = entity->getComponents();
    for (auto id : components) {
        entityComponentManager->deleteEntityComponent(id);
    }
}
