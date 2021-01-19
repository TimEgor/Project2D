#pragma once

#include <EntityManager/EntityComponent.h>

class EntityComponent;
class EntityComponentManager;

class EntityComponentHandler final {
private:
	EntityComponent& component;
	size_t componentAllocatorIndex;
	EntityComponentID id;

public:
	EntityComponentHandler(EntityComponentID id, EntityComponent &component, size_t componentAllocatorIndex)
		: component(component), componentAllocatorIndex(componentAllocatorIndex), id(id) {
		component.handler = this;
	}
	EntityComponentHandler(const EntityComponentHandler&) = delete;
	EntityComponentHandler(EntityComponentHandler&&) = delete;

	EntityComponent& getComponent() { return component; }
	EntityComponentID getID() { return id; }

	size_t getComponentAllocatorIndex() { return componentAllocatorIndex; }
};