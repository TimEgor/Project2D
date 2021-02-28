#pragma once

#include <EntityManager/EntityComponent.h>

#include <vector>

class EntityHandler;

class Entity final {
	friend EntityHandler;

private:
	std::vector<EntityComponentID> componentsIDs;
	EntityHandler* handler;

public:
	Entity() : handler(nullptr) {}

	void addComponent(EntityComponentID id);
	void removeComponent(EntityComponentID id);
	void removeAllComponents();

	bool isComponentContained(EntityComponentID id);

	EntityHandler* getHandler() const { return handler; }
	EntityID getID() const;

	const std::vector<EntityComponentID>& getComponents();
	void getComponents(std::vector<EntityComponentID>& container);
};