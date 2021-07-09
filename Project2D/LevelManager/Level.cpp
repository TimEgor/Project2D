#include "Level.h"

#include <EntityManager/EntityComponents/SpriteRendererEntityComponent.h>
#include <EntityManager/EntityComponents/Canvas/CanvasSpriteRendererEntityComponent.h>
#include <EntityManager/EntityComponents/Canvas/CanvasLabelEntityComponent.h>
#include <GameLogic/CPP/CppGameLogicClassGroupManager.h>
#include <Graphics/Scene.h>
#include <Graphics/FontManager.h>
#include <Graphics/Resources/MaterialResource.h>
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

    cppGameLogicClassGroupManager = new CppGameLogicClassGroupManager();
    if (!cppGameLogicClassGroupManager) {
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

    if (cppGameLogicClassGroupManager) {
        delete cppGameLogicClassGroupManager;
        cppGameLogicClassGroupManager = nullptr;
    }
}

void Level::update(float deltaTime) {
    if (cppGameLogicClassGroupManager) {
        cppGameLogicClassGroupManager->update(deltaTime);
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

//RenderingData& Level::getRenderingData() {
//    SceneRenderingOrderManager& sceneRedneringOrderManager = renderingData->getSceneRedneringOrderManager();
//    CanvasRenderingOrderManager& canvasRedneringOrderManager = renderingData->getCanvasRedneringOrderManager();
//
//    sceneRedneringOrderManager.clear();
//    canvasRedneringOrderManager.clear();
//
//    size_t sceneRenderingNodeNum = entityComponentManager->getEntityComponentsNum(SpriteRendererEntityComponentType);
//    if (sceneRenderingNodeNum != 0) {
//        sceneRedneringOrderManager.reserve(sceneRenderingNodeNum);
//
//        if (auto spritesAllocators = entityComponentManager->getEntityComponents(SpriteRendererEntityComponentType)) {
//            RenderingOrder<SpriteRenderingOrderNode>& order = sceneRedneringOrderManager.getSpriteRenderingOrder();
//            order.reserve(entityComponentManager->getEntityComponentsNum(SpriteRendererEntityComponentType));
//
//            size_t forwardNodeIndex = sceneRedneringOrderManager.getForwardNodeIndex();
//            size_t reverseNodeIndex = sceneRedneringOrderManager.getReverseNodeIndex();
//
//            size_t allocatorsNum = spritesAllocators->size();
//            for (size_t allocatorIndex = 0; allocatorIndex < allocatorsNum; ++allocatorIndex) {
//                const ArrayPoolAllocator& allocator = (*spritesAllocators)[allocatorIndex];
//                size_t allocatorSize = allocator.size();
//
//                for (size_t componentIndex = 0; componentIndex < allocatorSize; ++componentIndex) {
//                    const SpriteRendererEntityComponent& component = allocator.getElement<SpriteRendererEntityComponent>(componentIndex);
//
//                    ResourceReference spriteResource = component.getSpriteResource();
//                    if (!spriteResource.isNull()) {
//                        Node* node = component.getParent()->getNode();
//                        node->updateTransform();
//
//                        TransformMatrix* worldTransform = node->getTransform()->getWorldTransformMatrix();
//
//                        ResourceReference materialResource = component.getMaterialResource();
//
//                        if (materialResource.getResource<MaterialResource>().getSortingMode() == ReverseMaterialSortingModeType) {
//                            TransformMatrix* renderingNodeTransformMatrix = sceneRedneringOrderManager.pushReversedTransformMatrix(*worldTransform);
//                            SpriteRenderingOrderNode renderingNode(renderingNodeTransformMatrix, materialResource, spriteResource);
//                            order.pushRenderingNode(renderingNode);
//
//                            sceneRedneringOrderManager.pushReverseNode(renderingNode);
//                        }
//                        else {
//                            TransformMatrix* renderingNodeTransformMatrix = sceneRedneringOrderManager.pushTransformMatrix(*worldTransform);
//                            SpriteRenderingOrderNode renderingNode(renderingNodeTransformMatrix, materialResource, spriteResource);
//                            order.pushRenderingNode(renderingNode);
//
//                            sceneRedneringOrderManager.pushForwardNode(renderingNode);
//                        }
//                    }
//                }
//            }
//
//            sceneRedneringOrderManager.sortRenderingNodes(forwardNodeIndex, sceneRedneringOrderManager.getForwardNodeIndex(), SpriteRenderingOrderNode::sortPredicate);
//            sceneRedneringOrderManager.sortRenderingNodes(sceneRedneringOrderManager.getReverseNodeIndex(), reverseNodeIndex, SpriteRenderingOrderNode::sortPredicate);
//        }
//    }

    //spritesNum = entityComponentManager->getEntityComponentsNum(CanvasSpriteRendererEntityComponentType);
    //spritesNum += entityComponentManager->getEntityComponentsNum(CanvasLabelEntityComponentType);
    //if (spritesNum != 0) {
    //    canvasRenderingOrder->reserve(spritesNum);

    //    if (auto spritesAllocators = entityComponentManager->getEntityComponents(CanvasSpriteRendererEntityComponentType)) {
    //        size_t allocatorsNum = spritesAllocators->size();
    //        for (size_t allocatorIndex = 0; allocatorIndex < allocatorsNum; ++allocatorIndex) {
    //            ArrayPoolAllocator& allocator = (*spritesAllocators)[allocatorIndex];
    //            size_t allocatorSize = allocator.size();

    //            for (size_t componentIndex = 0; componentIndex < allocatorSize; ++componentIndex) {
    //                CanvasSpriteRendererEntityComponent& component = allocator.getElement<CanvasSpriteRendererEntityComponent>(componentIndex);

    //                ResourceReference spriteResource = component.getSpriteResource();
    //                if (!spriteResource.isNull()) {
    //                    Node* node = component.getParent()->getNode();
    //                    node->updateTransform();

    //                    Transform* transform = node->getTransform();
    //                    TransformMatrix worldTransform = *(transform->getWorldTransformMatrix());

    //                    if (transform->getTransformType() == CanvasTransformType) {
    //                        worldTransform.m[0][0] *= ((CanvasTransform*)(transform))->getWidth();
    //                        worldTransform.m[1][1] *= ((CanvasTransform*)(transform))->getHeight();
    //                    }             

    //                    canvasRenderingOrder->pushNode(component.getMaterialResource(), spriteResource, &worldTransform);
    //                }
    //            }
    //        }
    //    }

    //    if (auto labelsAllocators = entityComponentManager->getEntityComponents(CanvasLabelEntityComponentType)) {
    //        FontManager& fontManager = FontManager::get();

    //        size_t allocatorsNum = labelsAllocators->size();
    //        for (size_t allocatorIndex = 0; allocatorIndex < allocatorsNum; ++allocatorIndex) {
    //            ArrayPoolAllocator& allocator = (*labelsAllocators)[allocatorIndex];
    //            size_t allocatorSize = allocator.size();

    //            for (size_t componentIndex = 0; componentIndex < allocatorSize; ++componentIndex) {
    //                CanvasLabelEntityComponent& component = allocator.getElement<CanvasLabelEntityComponent>(componentIndex);

    //                if (!component.getText().empty()) {
    //                    component.update();

    //                    ResourceReference spriteResource = fontManager.getFontInfo(component.getFontID())->bitmapTexture;

    //                    Node* node = component.getParent()->getNode();
    //                    node->updateTransform();

    //                    Transform* transform = node->getTransform();
    //                    TransformMatrix worldTransform = *(transform->getWorldTransformMatrix());

    //                    canvasRenderingOrder->pushBatchNode(component.getMaterialResource(), spriteResource, &worldTransform,
    //                        component.getVertecesCount(), component.getVerteces(), component.getIndecesCount(), component.getIndeces());
    //                }
    //            }
    //        }
    //    }
    //}

//    return *renderingData;
//}
