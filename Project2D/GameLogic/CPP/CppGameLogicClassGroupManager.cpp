#include "CppGameLogicClassGroupManager.h"
#pragma once

bool CppGameLogicClassGroupManager::init(std::vector<int> orderIndecies) {
	for (auto index : orderIndecies) {
		auto newElementIter = groups.emplace(std::piecewise_construct, std::forward_as_tuple(index), std::forward_as_tuple());
		newElementIter.first->second.init(index);
	}

	return true;
}

void CppGameLogicClassGroupManager::release() {
	groups = std::map<int, CppGameLogicClassGroup>();
}

CppGameLogicClassGroup* CppGameLogicClassGroupManager::addNewGroup(int orderIndex) {
	auto newElementIter = groups.emplace(std::piecewise_construct, std::forward_as_tuple(orderIndex), std::forward_as_tuple());
	newElementIter.first->second.init(orderIndex);

	return &newElementIter.first->second;
}

CppGameLogicClassGroup::ClassStorageInfo CppGameLogicClassGroupManager::addComponentToGroup(CppGameLogicClass* component, int orderIndex) {
	CppGameLogicClassGroup* group = nullptr;

	auto findIter = groups.find(orderIndex);
	if (findIter == groups.end()) {
		group = addNewGroup(orderIndex);
	}
	else {
		group = &findIter->second;
	}

	return group->addComponentToGroup(component);
}

void CppGameLogicClassGroupManager::removeComponentFromGroup(CppGameLogicClass** component, size_t allocatorID, int orderIndex) {
	auto findIter = groups.find(orderIndex);
	if (findIter != groups.end()) {
		findIter->second.removeComponentFromGroup(component, allocatorID);
	}
}

void CppGameLogicClassGroupManager::removeComponentFromGroup(const CppGameLogicClassGroup::ClassStorageInfo& storageInfo, int orderIndex) {
	removeComponentFromGroup(storageInfo.componentPtr, storageInfo.allocatorID, orderIndex);
}

void CppGameLogicClassGroupManager::update(float deltaTime) {
	for (auto& group : groups) {
		group.second.update(deltaTime);
	}
}
