#pragma once

#include <EntityManager/EntityComponent.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>

class EntityComponent;
class EntityComponentManager;
class EntityComponentReferenceHandler;
class EntityComponentReference;
class Level;

class EntityComponentHandler final {
private:
	EntityComponent* component = nullptr;
	Level* level = nullptr;
	EntityComponentReferenceHandler* reference = nullptr;
	AllocatorID allocatorID = 0;
	EntityComponentID id = 0;

public:
	EntityComponentHandler(EntityComponentID id, EntityComponent *component, size_t componentAllocatorID, Level* level)
		: component(component), level(level), allocatorID(componentAllocatorID), id(id) {
		component->handler = this;
	}
	EntityComponentHandler(const EntityComponentHandler&) = delete;
	EntityComponentHandler(EntityComponentHandler&&) = delete;

	EntityComponent* getComponent() { return component; }
	void setComponent(EntityComponent* newComponent) { component = newComponent; }

	EntityComponentID getID() { return id; }
	void setParent(Entity* parent) { component->entity = parent; }

	EntityComponentReference getReference();
	EntityComponentReferenceHandler* getReferenceHandler() { return reference; }

	Level* getLevel() { return level; }

	size_t getComponentAllocatorID() { return allocatorID; }
};