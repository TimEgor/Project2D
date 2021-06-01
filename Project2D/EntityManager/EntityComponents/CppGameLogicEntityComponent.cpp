#include "CppGameLogicEntityComponent.h"

#include <GameLogic/CPP/CppGameLogicClassManager.h>
#include <MemoryManager/MemoryCore.h>

#include <cassert>

CppGameLogicEntityComponent::CppGameLogicEntityComponent(const char* gameLogicClassName) {
	gameLogicClass = CppGameLogicClassManager::get().createComponent(gameLogicClassName, this);
	assert(gameLogicClass);
}

CppGameLogicEntityComponent::CppGameLogicEntityComponent(CppClassNameHash hash) {
	gameLogicClass = CppGameLogicClassManager::get().createComponent(hash, this);
	assert(gameLogicClass);
}

void CppGameLogicEntityComponent::release() {
	assert(gameLogicClass);
	memRelease(gameLogicClass);
}

void CppGameLogicEntityComponent::update(float deltaTime) {
	gameLogicClass->update(deltaTime);
}
