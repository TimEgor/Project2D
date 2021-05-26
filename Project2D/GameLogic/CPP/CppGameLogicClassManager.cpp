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

CppGameLogicEntityComponent* CppGameLogicClassManager::createComponent(const char* className, Heap* heap) {
	assert(className);
	size_t classNameLength = strlen(className);
	assert(classNameLength);

	return createComponent(Crc32(className, classNameLength), heap);
}

CppGameLogicEntityComponent* CppGameLogicClassManager::createComponent(ClassNameHash classNameHash, Heap* heap) {
	auto findIter = classes.find(classNameHash);
	if (findIter != classes.end()) {
		return findIter->second(heap);
	}

	return nullptr;
}
