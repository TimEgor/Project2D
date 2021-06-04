#pragma once

#include <GameLogic/CPP/CppGameLogicClassGroup.h>

#include <map>

class CppGameLogicClassGroupManager final {
private:
	std::map<int, CppGameLogicClassGroup> groups;

public:
	CppGameLogicClassGroupManager() = default;

	bool init(std::vector<int> orderIndecies);
	void release();

	CppGameLogicClassGroup* addNewGroup(int orderIndex);

	CppGameLogicClassGroup::ClassStorageInfo addComponentToGroup(CppGameLogicClass* component, int orderIndex);
	void removeComponentFromGroup(CppGameLogicClass** component, size_t allocatorID, int orderIndex);
	void removeComponentFromGroup(const CppGameLogicClassGroup::ClassStorageInfo& storageInfo, int orderIndex);

	void update(float deltaTime);
};