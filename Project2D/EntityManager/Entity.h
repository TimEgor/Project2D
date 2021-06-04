#pragma once

#include <EntityManager/EntityComponent.h>

#include <vector>

class EntityHandler;
class Node;

class Entity final {
	friend EntityHandler;

private:
	std::vector<EntityComponentID> componentsIDs;
	EntityHandler* handler = nullptr;
	Node* node = nullptr;

public:
	Entity(EntityHandler* handler) : handler(handler) {}

	void addComponent(EntityComponentID id);
	void removeComponent(EntityComponentID id);
	void removeAllComponents();

	bool isComponentContained(EntityComponentID id);

	EntityHandler* getHandler() const { return handler; }
	EntityID getID() const;

	Node* getNode() const { return node; }
	void setNode(Node* newNode) { node = newNode; }

	const std::vector<EntityComponentID>& getComponents();
	void getComponents(std::vector<EntityComponentID>& container);
};