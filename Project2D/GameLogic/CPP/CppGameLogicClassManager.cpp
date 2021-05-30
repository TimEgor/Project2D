#include "CppGameLogicClassManager.h"

#include <Utilities/HashFunction/CRC32.h>

#include <cassert>

bool CppGameLogicClassManager::registryNewCreatingFunction(const char* className, CppGameLogicEntityComponentCreatingFunc function) {
	assert(className);
	size_t classNameLength = strlen(className);
	assert(classNameLength);

	auto insertingPair = classes.insert(std::make_pair(Crc32(className, classNameLength), function));
	return insertingPair.second;
}

CppGameLogicClassManager& CppGameLogicClassManager::get() {
	static CppGameLogicClassManager uniqueCppGameLogicClassManager;
	return uniqueCppGameLogicClassManager;
}

bool CppGameLogicClassManager::init() {
	return true;
}

void CppGameLogicClassManager::release() {
}

CppGameLogicClass* CppGameLogicClassManager::createComponent(const char* className, Heap* heap, CppGameLogicEntityComponent* component) {
	assert(className);
	size_t classNameLength = strlen(className);
	assert(classNameLength);

	return createComponent(Crc32(className, classNameLength), heap, component);
}

CppGameLogicClass* CppGameLogicClassManager::createComponent(CppClassNameHash classNameHash, Heap* heap, CppGameLogicEntityComponent* component) {
	auto findIter = classes.find(classNameHash);
	if (findIter != classes.end()) {
		CppGameLogicClass* newGameLogicClass = findIter->second(heap, component);
		newGameLogicClass->setComponent(component);

		return newGameLogicClass;
	}

	return nullptr;
}
