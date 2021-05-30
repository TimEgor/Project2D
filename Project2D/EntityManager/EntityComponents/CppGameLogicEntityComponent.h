#pragma once

#include <EntityManager/EntityComponents/GameLogicEntityComponent.h>
#include <GameLogic/CPP/CppGameLogicClass.h>

class CppGameLogicEntityComponent final : public GameLogicEntityComponent {
private:
	CppGameLogicClass* gameLogicClass;

public:
	CppGameLogicEntityComponent(const char* gameLogicClassName);
	CppGameLogicEntityComponent(CppClassNameHash hash);
	~CppGameLogicEntityComponent() { release(); }

	virtual void release() override;

	virtual void update(float deltaTime) override;

	virtual EntityComponentType getEntityComponentType() const override { return CppGameLogicEntityComponentType; }
	static EntityComponentType getType() { return CppGameLogicEntityComponentType; }
};

//template<>
//class ComponentCreator<CppGameLogicEntityComponent> final {
//public:
//	template <typename... Args>
//	static CppGameLogicEntityComponent* createComponent(ComponentAllocators::AllocationInfo allocationInfo, Args... args) {
//		return CppGameLogicClassManager::get().createComponent("TestClass", MemoryManager::get().getDefaultHeap());
//	}
//};