#include "Entity.h"

#include <EntityManager/EntityHandler.h>
#include <EntityManager/EntityComponentManager.h>
#include <BaseGameLogic/LevelManager.h>

#include <cassert>

void Entity::addComponent(EntityComponent* component) {
	assert(component->parent == nullptr);
	component->parent = this;

	components.push_back(component);
}

void Entity::removeComponent(EntityComponentID id, bool withDeleting) {
	assert(id != 0);

	auto findIter = std::find_if(components.begin(), components.end(),
		[id](const EntityComponent* component) -> bool {
			return component->getID() == id;
		});

	if (findIter != components.end()) {
		EntityComponent* component = (*findIter);
		component->parent = nullptr;

		if (component->getHandler() && withDeleting) {
			LevelManager::get().getCurrentLevel().getEntityComponentManager().deleteEntityComponent(component);
		}

		std::iter_swap(findIter, components.rbegin());
		components.pop_back();
	}
}

void Entity::removeComponent(EntityComponent* component, bool withDeleting) {
	assert(component);

	auto findIter = std::find(components.begin(), components.end(), component);

	if (findIter != components.end()) {
		component->parent = nullptr;

		if (component->getHandler() && withDeleting) {
			LevelManager::get().getCurrentLevel().getEntityComponentManager().deleteEntityComponent(component);
		}

		std::iter_swap(findIter, components.rbegin());
		components.pop_back();
	}
}

void Entity::removeAllComponents() {
	for (auto* component : components) {
		if (component->getHandler()) {
			LevelManager::get().getCurrentLevel().getEntityComponentManager().deleteEntityComponent(component);
		}
	}

	components = std::vector<EntityComponent*>();
}

EntityID Entity::getID() const {
	if (handler) {
		return handler->getID();
	}

	return 0;
}

EntityComponent* Entity::getComponent(EntityComponentID id) {
	auto findIter = std::find_if(components.begin(), components.end(),
		[id](const EntityComponent* reference) -> bool {
			return reference->getID() == id;
		});

	if (findIter == components.end()) {
		return nullptr;
	}

	return *findIter;
}

const std::vector<EntityComponent*>& Entity::getComponents() {
	return components;
}

void Entity::getComponents(std::vector<EntityComponent*>& container) {
	container.resize(container.size());
	std::copy(components.begin(), components.end(), container.begin());
}
