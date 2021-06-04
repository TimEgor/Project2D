#pragma once

#include <EntityManager/EntityComponents/GameLogicEntityComponent.h>
#include <GameLogic/CPP/CppGameLogicClass.h>
#include <GameLogic/CPP/CppGameLogicClassGroup.h>

class CppGameLogicEntityComponent final : public GameLogicEntityComponent {
private:
	CppGameLogicClass* gameLogicClass;
	CppGameLogicClassGroup::ClassStorageInfo groupStorageInfo;

public:
	CppGameLogicEntityComponent(EntityComponentHandler* handler, const char* gameLogicClassName);
	CppGameLogicEntityComponent(EntityComponentHandler* handler, CppClassNameHash hash);
	~CppGameLogicEntityComponent() { release(); }

	virtual void release() override;

	virtual void update(float deltaTime) override;

	virtual EntityComponentType getEntityComponentType() const override { return CppGameLogicEntityComponentType; }
	static EntityComponentType getType() { return CppGameLogicEntityComponentType; }
};