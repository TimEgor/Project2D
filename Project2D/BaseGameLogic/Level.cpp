#include "Level.h"

#include <EntityManager/EntityManager.h>
#include <EntityManager/EntityComponentManager.h>
#include <Graphics/Scene.h>

bool Level::init() {
    entityManager = new EntityManager();
    entityComponentManager = new EntityComponentManager();
    scene = new Scene();

    if (!entityManager || !entityComponentManager || !scene) {
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
