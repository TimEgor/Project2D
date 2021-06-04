#include "CppGameLogicEntityComponent.h"

#include <EntityManager/EntityComponentHandler.h>
#include <GameLogic/CPP/CppGameLogicClassManager.h>
#include <GameLogic/CPP/CppGameLogicClassGroupManager.h>
#include <LevelManager/Level.h>

#include <cassert>

CppGameLogicEntityComponent::CppGameLogicEntityComponent(EntityComponentHandler* handler, const char* gameLogicClassName)
	: GameLogicEntityComponent(handler) {
	gameLogicClass = CppGameLogicClassManager::get().createComponent(gameLogicClassName, this);
	assert(gameLogicClass);

	groupStorageInfo = handler->getLevel()->getCppGameLogicClassGroupManager()->addComponentToGroup(gameLogicClass, gameLogicClass->getOrderIndex());
}

CppGameLogicEntityComponent::CppGameLogicEntityComponent(EntityComponentHandler* handler, CppClassNameHash hash)
	: GameLogicEntityComponent(handler) {
	gameLogicClass = CppGameLogicClassManager::get().createComponent(hash, this);
	assert(gameLogicClass);

	groupStorageInfo = handler->getLevel()->getCppGameLogicClassGroupManager()->addComponentToGroup(gameLogicClass, gameLogicClass->getOrderIndex());
}

void CppGameLogicEntityComponent::release() {
	assert(gameLogicClass);

	handler->getLevel()->getCppGameLogicClassGroupManager()->removeComponentFromGroup(groupStorageInfo, gameLogicClass->getOrderIndex());
	delete gameLogicClass;
}

void CppGameLogicEntityComponent::update(float deltaTime) {
	gameLogicClass->update(deltaTime);
}
