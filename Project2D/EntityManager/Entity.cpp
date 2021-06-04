#include "Entity.h"

#include <EntityManager/EntityHandler.h>
#include <EntityManager/EntityComponentManager.h>
#include <LevelManager/LevelManager.h>

#include <cassert>

void Entity::addComponent(EntityComponentID id) {
	assert(id != 0);
	componentsIDs.push_back(id);
}

void Entity::removeComponent(EntityComponentID id) {
	assert(id != 0);

	auto findIter = std::find(componentsIDs.begin(), componentsIDs.end(), id);

	if (findIter != componentsIDs.end()) {
		std::iter_swap(findIter, componentsIDs.rbegin());
		componentsIDs.pop_back();
	}
}

void Entity::removeAllComponents() {
	componentsIDs = std::vector<EntityComponentID>();
}

bool Entity::isComponentContained(EntityComponentID id) {
	auto findIter = std::find(componentsIDs.begin(), componentsIDs.end(), id);

	if (findIter != componentsIDs.end()) {
		return true;
	}

	return false;
}

EntityID Entity::getID() const {
	if (handler) {
		return handler->getID();
	}

	return 0;
}

const std::vector<EntityComponentID>& Entity::getComponents() {
	return componentsIDs;
}

void Entity::getComponents(std::vector<EntityComponentID>& container) {
	container.resize(container.size());
	std::copy(componentsIDs.begin(), componentsIDs.end(), container.begin());
}
