#pragma once

#include <EntityManager/EntityComponent.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>

class EntityComponent;
class EntityComponentManager;
class BaseGameSpace;

class EntityComponentHandler final {
private:
	EntityComponent* component;
	BaseGameSpace* level;
	AllocatorID allocatorID;
	EntityComponentID id;

public:
	EntityComponentHandler(EntityComponentID id, EntityComponent *component, size_t componentAllocatorID, BaseGameSpace* level)
		: component(component), level(level), allocatorID(componentAllocatorID), id(id) {
		component->handler = this;
	}
	EntityComponentHandler(const EntityComponentHandler&) = delete;
	EntityComponentHandler(EntityComponentHandler&&) = delete;

	EntityComponent* getComponent() { return component; }
	void setComponent(EntityComponent* newComponent) { component = newComponent; }

	EntityComponentID getID() { return id; }
	void setParentID(EntityID parentID) { component->parentID = parentID; }

	size_t getComponentAllocatorID() { return allocatorID; }
};