#pragma once

#include <EntityManager/EntityComponent.h>

class EntityComponent;
class EntityComponentManager;

class EntityComponentHandler final {
private:
	EntityComponent& component;
	EntityComponentManager& manager;
	size_t componentAllocatorIndex;
	EntityComponentID id;

public:
	EntityComponentHandler(EntityComponentID id, EntityComponent &component, EntityComponentManager& manager, size_t componentAllocatorIndex)
		: component(component), manager(manager), componentAllocatorIndex(componentAllocatorIndex), id(id) {
		component.handler = this;
	}
	EntityComponentHandler(const EntityComponentHandler&) = delete;
	EntityComponentHandler(EntityComponentHandler&& handler) = delete;

	EntityComponent& getComponent() { return component; }
	EntityComponentID getID() { return id; }

	EntityComponentManager& getManager() { return manager; }

	size_t getComponentAllocatorIndex() { return componentAllocatorIndex; }
};