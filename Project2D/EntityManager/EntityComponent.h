#pragma once

#include <EntityManager/EntityComponentType.h>
#include <EntityManager/EntityManagerTypes.h>

class Entity;
class EntityComponentHandler;

class EntityComponent {
	friend EntityComponentHandler;

protected:
	EntityID parentID;
	EntityComponentHandler *handler;

public:
	EntityComponent() : parentID(0), handler(nullptr) {}
	virtual ~EntityComponent() {}

	EntityComponentHandler* getHandler() const { return handler; }
	EntityComponentID getID() const;
	EntityID getParentID() const { return parentID; }

	virtual EntityComponentType getEntityComponentType() const = 0;
};