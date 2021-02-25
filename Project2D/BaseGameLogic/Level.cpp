#include "Level.h"

#include <EntityManager/EntityManager.h>
#include <EntityManager/EntityComponentManager.h>
#include <Graphics/Scene.h>

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

    if (!scene->init()) {
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
}

Entity* Level::createEntity() {
    Entity* entity = entityManager->createEntity();
    scene->createNode(entity->getID());

    return entity;
}

void Level::deleteEntity(Entity* entity) {
    auto components = entity->getComponents();
    for (auto id : components) {
        entityComponentManager->deleteEntityComponent(id);
    }

    scene->deleteNode(entity->getID());
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
