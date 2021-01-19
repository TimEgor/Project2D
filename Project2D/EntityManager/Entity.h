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
	void removeComponent(EntityComponentID id, bool withDeleting = true);
	void removeComponent(EntityComponent* component, bool withDeleting = true);
	void removeAllComponents();

	EntityHandler* getHandler() const { return handler; }
	EntityID getID() const;

	EntityComponent* getComponent(EntityComponentID id);

	const std::vector<EntityComponent*>& getComponents();
	void getComponents(std::vector<EntityComponent*>& container);
};