#include "CppGameLogicEntityComponent.h"

#include <GameLogic/CPP/CppGameLogicClassManager.h>
#include <MemoryManager/MemoryManager.h>

#include <cassert>

CppGameLogicEntityComponent::CppGameLogicEntityComponent(const char* gameLogicClassName) {
	gameLogicClass = CppGameLogicClassManager::get().createComponent(gameLogicClassName, MemoryManager::get().getDefaultHeap(), this);
	assert(gameLogicClass);
}

CppGameLogicEntityComponent::CppGameLogicEntityComponent(CppClassNameHash hash) {
	gameLogicClass = CppGameLogicClassManager::get().createComponent(hash, MemoryManager::get().getDefaultHeap(), this);
	assert(gameLogicClass);
}

void CppGameLogicEntityComponent::release() {
	assert(gameLogicClass);
	MemoryManager::get().getDefaultHeap()->deallocate(gameLogicClass);
}

void CppGameLogicEntityComponent::update(float deltaTime) {
	gameLogicClass->update(deltaTime);
}
