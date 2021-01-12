#pragma once

#include <EntityManager/EntityComponent.h>

#include <vector>
#include <cstdint>

typedef uint32_t EntityID;

class EntityHandler;

class Entity final {
	friend EntityHandler;

private:
	std::vector<EntityComponent*> components;
	EntityHandler* handler;

public:
	Entity() : handler(nullptr) {}

	void addComponent(EntityComponent* component);
	void removeComponent(EntityComponentID id);
	void removeComponentWithoutDestruction(EntityComponentID id);
	void removeAllComponents();

	EntityHandler* getHandler() const { return handler; }
	EntityID getID() const;

	EntityComponent* getComponent(EntityComponentID id);

	void getComponents(std::vector<EntityComponent*>& container);
};