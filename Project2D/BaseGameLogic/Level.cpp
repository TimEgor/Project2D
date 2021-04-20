#include "Level.h"

#include <EntityManager/EntityManager.h>
#include <EntityManager/EntityComponentManager.h>
#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <EntityManager/EntityComponents/Canvas/CanvasSpriteRendererEntityComponent.h>
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

    sceneRenderingOrder = new RenderingOrder();
    if (!sceneRenderingOrder->init(MemoryManager::get().getDefaultHeap())) {
        release();
        return false;
    }

    canvasRenderingOrder = new RenderingOrder();
    if (!canvasRenderingOrder->init(MemoryManager::get().getDefaultHeap())) {
        release();
        return false;
    }

    return true;
}

void Level::release() {
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

    if (scene) {
        scene->release();
        delete scene;
        scene = nullptr;
    }

    if (sceneRenderingOrder) {
        sceneRenderingOrder->release();
        delete sceneRenderingOrder;
        sceneRenderingOrder = nullptr;
    }

    if (canvasRenderingOrder) {
        canvasRenderingOrder->release();
        delete canvasRenderingOrder;
        canvasRenderingOrder = nullptr;
    }
}

Entity* Level::createSceneEntity() {
    Entity* entity = entityManager->createEntity();
    Node* node = scene->createSceneNode(entity);
    entity->setNode(node);

    return entity;
}

Entity* Level::createSceneEntity(Entity* parent) {
    return createSceneEntity(parent->getID());
}

Entity* Level::createSceneEntity(EntityID parentID) {
    Entity* entity = entityManager->createEntity();
    Node* node = scene->createSceneNode(entity, parentID);
    entity->setNode(node);

    return entity;
}

Entity* Level::createCanvasEntity() {
    Entity* entity = entityManager->createEntity();
    Node* node = scene->createCanvasNode(entity);
    entity->setNode(node);

    return entity;
}

Entity* Level::createCanvasEntity(Entity* parent) {
    return createCanvasEntity(parent->getID());
}

Entity* Level::createCanvasEntity(EntityID parentID) {
    Entity* entity = entityManager->createEntity();
    Node* node = scene->createCanvasNode(entity, parentID);
    entity->setNode(node);

    return entity;
}

void Level::deleteEntity(Entity* entity) {
    deleteEntityComponentsFromEntity(entity);

    scene->deleteNode(entity->getID());
    entityManager->deleteEntity(entity);
}

void Level::deleteEntity(EntityID id) {
    Entity* entity = entityManager->getEntity(id);
    deleteEntity(entity);
}

EntityComponent* Level::createEntityComponent(EntityComponentType type, Entity* parent) {
    assert(parent);

    EntityComponent* component = nullptr;

    switch (type) {
    case SpriteRendererEntityComponentType:
        component = (EntityComponent*)(entityComponentManager->createComponent<SpriteRendererEntityComponent>(type));
        break;
    case CanvasSpriteRendererEntityComponentType:
        component = (EntityComponent*)(entityComponentManager->createComponent<CanvasSpriteRendererEntityComponent>(type));
        break;
    default:
        break;
    }

    parent->addComponent(component->getID());
    component->getHandler()->setParent(parent);

    return component;
}

EntityComponent* Level::createEntityComponent(EntityComponentType type, EntityID parentID) {
    Entity* entity = entityManager->getEntity(parentID);
    return createEntityComponent(type, entity);
}

void Level::deleteEntityComponent(EntityComponent* component) {
    assert(component);

    component->getParent()->removeComponent(component->getID());

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
    renderingData.setSceneRenderingOrder(sceneRenderingOrder);
    renderingData.setCanvasRenderingOrder(canvasRenderingOrder);

    sceneRenderingOrder->clear();
    canvasRenderingOrder->clear();

    size_t spritesNum = entityComponentManager->getEntityComponentsNum(SpriteRendererEntityComponentType);
    if (spritesNum != 0) {
        sceneRenderingOrder->preReSize(spritesNum);

        if (auto spritesAllocators = entityComponentManager->getEntityComponents(SpriteRendererEntityComponentType)) {
            size_t allocatorsNum = spritesAllocators->size();
            for (size_t allocatorIndex = 0; allocatorIndex < allocatorsNum; ++allocatorIndex) {
                const ArrayPoolAllocator& allocator = (*spritesAllocators)[allocatorIndex];
                size_t allocatorSize = allocator.size();

                for (size_t componentIndex = 0; componentIndex < allocatorSize; ++componentIndex) {
                    const SpriteRendererEntityComponent& component = allocator.getElement<SpriteRendererEntityComponent>(componentIndex);

                    ResourceReference spriteResource = component.getSpriteResource();
                    if (!spriteResource.isNull()) {
                        Node* node = component.getParent()->getNode();
                        node->updateTransform();

                        TransformMatrix* worldTransform = node->getTransform()->getWorldTransformMatrix();

                        sceneRenderingOrder->pushNode(node->getID(), component.getMaterialResource(), spriteResource, worldTransform);
                    }
                }
            }
        }
    }

    spritesNum = entityComponentManager->getEntityComponentsNum(CanvasSpriteRendererEntityComponentType);
    if (spritesNum != 0) {
        canvasRenderingOrder->preReSize(spritesNum);

        if (auto spritesAllocators = entityComponentManager->getEntityComponents(CanvasSpriteRendererEntityComponentType)) {
            size_t allocatorsNum = spritesAllocators->size();
            for (size_t allocatorIndex = 0; allocatorIndex < allocatorsNum; ++allocatorIndex) {
                const ArrayPoolAllocator& allocator = (*spritesAllocators)[allocatorIndex];
                size_t allocatorSize = allocator.size();

                for (size_t componentIndex = 0; componentIndex < allocatorSize; ++componentIndex) {
                    const CanvasSpriteRendererEntityComponent& component = allocator.getElement<CanvasSpriteRendererEntityComponent>(componentIndex);

                    ResourceReference spriteResource = component.getSpriteResource();
                    if (!spriteResource.isNull()) {
                        Node* node = component.getParent()->getNode();
                        node->updateTransform();
                        
                        Transform* transform = node->getTransform();
                        if (transform->getTransformType() == CanvasTransformType) {
                            ((CanvasTransform*)(transform))->prepareForRenderingCanvas();
                        }

                        TransformMatrix* worldTransform = transform->getWorldTransformMatrix();

                        canvasRenderingOrder->pushNode(node->getID(), component.getMaterialResource(), spriteResource, worldTransform);
                    }
                }
            }
        }
    }

    return renderingData;
}
