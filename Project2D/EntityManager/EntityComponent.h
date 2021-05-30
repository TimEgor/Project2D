#pragma once

#include <EntityManager/EntityComponentType.h>
#include <EntityManager/EntityManagerTypes.h>

class Entity;
class EntityComponentHandler;

class EntityComponent {
	friend EntityComponentHandler;

protected:
	EntityComponentHandler* handler = nullptr;
	Entity* entity = nullptr;

public:
	EntityComponent() = default;
	virtual ~EntityComponent() {}

	virtual void release() {}

	EntityComponentHandler* getHandler() const { return handler; }
	EntityComponentID getID() const;
	Entity* getParent() const { return entity; }

	virtual EntityComponentType getEntityComponentType() const = 0;
};