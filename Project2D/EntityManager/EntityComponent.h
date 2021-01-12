#pragma once

#include <EntityManager/EntityComponentType.h>

#include <cstdint>

typedef uint32_t EntityComponentID;

class Entity;
class EntityComponentHandler;

class EntityComponent {
	friend Entity;
	friend EntityComponentHandler;

protected:
	Entity* parent;
	EntityComponentHandler *handler;

public:
	EntityComponent() : parent(nullptr), handler(nullptr) {}
	virtual ~EntityComponent() = 0;

	EntityComponentHandler* getHandler() const { return handler; }
	EntityComponentID getID() const;
	Entity* getParent() const { return parent; }

	virtual EntityComponentType getEntityComponentType() = 0;
};