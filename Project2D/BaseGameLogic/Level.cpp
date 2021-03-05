#include "Level.h"

#include <EntityManager/EntityManager.h>
#include <EntityManager/EntityComponentManager.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <Graphics/Scene.h>
#include <MemoryManager/MemoryManager.h>
#include <ResourceManager/ResourceManager.h>

#include <cassert>

bool Level::init() {
    entityManager = new EntityManager();
    entityComponentManager = new EntityComponentManager();
    scene = new Scene();

    if (!entityManager || !entityComponentManager || !scene) {
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

    if (!scene->init(this)) {
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

void Level::release() {
    if (entityManager) {
        delete entityManager;
        entityManager = nullptr;
    }

    if (entityComponentManager) {
        delete entityComponentManager;
        entityComponentManager = nullptr;
    }

    if (scene) {
        delete scene;
        scene = nullptr;
    }

    if (renderingOrder) {
        delete renderingOrder;
        renderingOrder = nullptr;
    }
}

Entity* Level::createEntity() {
    Entity* entity = entityManager->createEntity();
    scene->createNode(entity->getID());

    return entity;
}

void Level::deleteEntity(Entity* entity) {
    deleteEntityComponentsFromEntity(entity);

    scene->deleteNode(entity->getID());
    entityManager->deleteEntity(entity);
}

void Level::deleteEntity(EntityID id) {
    Entity* entity = entityManager->getEntity(id);
    deleteEntity(id);
}

EntityComponent* Level::createEntityComponent(EntityComponentType type, Entity* parent) {
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

EntityComponent* Level::createEntityComponent(EntityComponentType type, EntityID parentID) {
    Entity* entity = entityManager->getEntity(parentID);
    return createEntityComponent(type, entity);
}

void Level::deleteEntityComponent(EntityComponent* component) {
    assert(component);

    Entity* entity = entityManager->getEntity(component->getParentID());
    entity->removeComponent(component->getID());

    entityComponentManager->deleteEntityComponent(component);
}

void Level::deleteEntityComponent(EntityComponentID id) {
    EntityComponent* component = entityComponentManager->getEntityComponent(id);
    deleteEntityComponent(component);
}

void Level::removeEntityComponentsFromEntity(Entity* entity) {
    deleteEntityComponentsFromEntity(entity);
    entity->removeAllComponents();
}

void Level::removeEntityComponentsFromEntity(EntityID id) {
    Entity* entity = entityManager->getEntity(id);
    removeEntityComponentsFromEntity(entity);
}

void Level::deleteEntityWithoutNode(Entity* entity) {
    deleteEntityComponentsFromEntity(entity);
    entityManager->deleteEntity(entity);
}

void Level::deleteEntityWithoutNode(EntityID id) {
    Entity* entity = entityManager->getEntity(id);
    deleteEntityWithoutNode(entity);
}

void Level::deleteEntityComponentsFromEntity(Entity* entity) {
    auto components = entity->getComponents();
    for (auto id : components) {
        entityComponentManager->deleteEntityComponent(id);
    }
}

RenderingData Level::getRenderingData() {
    RenderingData renderingData;
    renderingData.setScene(scene);
    renderingData.setRenderingOrder(renderingOrder);

    renderingOrder->clear();

    size_t spritesNum = entityComponentManager->getEntityComponentsNum(SpriteRendererEntityComponentType);
    if (spritesNum != 0) {
        renderingOrder->preReSize(spritesNum);

        if (auto spritesAllocators = entityComponentManager->getEntityComponents(SpriteRendererEntityComponentType)) {
            size_t allocatorsNum = spritesAllocators->size();
            for (size_t allocatorIndex = 0; allocatorIndex < allocatorsNum; ++allocatorIndex) {
                const ArrayPoolAllocator& allocator = (*spritesAllocators)[allocatorIndex];
                size_t allocatorSize = allocator.size();

                for (size_t componentIndex = 0; componentIndex < allocatorSize; ++componentIndex) {
                    const SpriteRendererEntityComponent& component = allocator.getElement<SpriteRendererEntityComponent>(componentIndex);

                    RenderingOrderNode node;
                    node.entityID = component.getParentID();
                    node.materialResource = component.getMaterialResource();
                    node.spriteResource = component.getSpriteResource();

                    if (!node.spriteResource.isNull()) {
                        renderingOrder->pushNode(node);
                    }
                }
            }
        }
    }

    return renderingData;
}
