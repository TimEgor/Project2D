#pragma once

#include <MemoryManager/Allocators/PoolAllocatorVector.h>
#include <MemoryManager/Allocators/ArrayPoolAllocator.h>

class CppGameLogicClass;

class CppGameLogicClassGroup final {
	typedef PoolAllocatorVector<ArrayPoolAllocator> ComponentPtrAllocators;

public:
	struct ClassStorageInfo final {
		CppGameLogicClass** componentPtr = nullptr;
		size_t allocatorID = 0;
	};

private:
	ComponentPtrAllocators allocators;

	int orderIndex = 0;

public:
	CppGameLogicClassGroup() = default;
	~CppGameLogicClassGroup() { release(); }

	bool init(int orderIndex);
	void release();

	ClassStorageInfo addComponentToGroup(CppGameLogicClass* component);
	void removeComponentFromGroup(CppGameLogicClass** component, size_t allocatorID);
	void removeComponentFromGroup(const ClassStorageInfo& storageInfo);

	void update(float deltaTime);
};