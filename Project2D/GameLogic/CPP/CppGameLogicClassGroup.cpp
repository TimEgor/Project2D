#include "CppGameLogicClassGroup.h"

#include <GameLogic/CPP/CppGameLogicClass.h>

bool CppGameLogicClassGroup::init(int index) {
	orderIndex = index;
	return allocators.init(sizeof(CppGameLogicClass*), 1024);
}

void CppGameLogicClassGroup::release() {
	orderIndex = 0;
	allocators.release();
}

CppGameLogicClassGroup::ClassStorageInfo CppGameLogicClassGroup::addComponentToGroup(CppGameLogicClass* component) {
	ComponentPtrAllocators::AllocationInfo info = allocators.allocate();
	CppGameLogicClass** ptr = (CppGameLogicClass**)(info.allocationAddress);

	*ptr = component;

	return ClassStorageInfo{ ptr, info.allocatorID };
}

void CppGameLogicClassGroup::removeComponentFromGroup(CppGameLogicClass** component, size_t allocatorID) {
	size_t allocatorIndex = allocators.getAllocatorIndexByID(allocatorID);
	ArrayPoolAllocator& allocator = allocators[allocatorIndex];
	allocator.deallocate(component);
}

void CppGameLogicClassGroup::removeComponentFromGroup(const ClassStorageInfo& storageInfo) {
	removeComponentFromGroup(storageInfo.componentPtr, storageInfo.allocatorID);
}

void CppGameLogicClassGroup::update(float deltaTime) {
	size_t allocatorsNum = allocators.size();
	for (size_t allocatorIndex = 0; allocatorIndex < allocatorsNum; ++allocatorIndex) {
		ArrayPoolAllocator& allocator = allocators[allocatorIndex];
		size_t allocatorSize = allocator.size();

		for (size_t logicClassIndex = 0; logicClassIndex < allocatorSize; ++logicClassIndex) {
			CppGameLogicClass* logicClassPtr = allocator.getElement<CppGameLogicClass*>(logicClassIndex);
			logicClassPtr->update(deltaTime);
		}
	}
}
