#pragma once

#include <EntityManager/EntityManagerTypes.h>
#include <EntityManager/EntityComponentType.h>
#include <cstdint>

typedef uint32_t GameSpaceID;

class EntityManager;
class EntityComponentManager;
class RenderingOrder;

class BaseGameSpace {
protected:
	EntityManager* entityManager;
	EntityComponentManager* entityComponentManager;

	RenderingOrder* renderingOrder;

	GameSpaceID spaceID;

	void deleteEntityComponentsFromEntity(Entity* entity);

public:
	BaseGameSpace(GameSpaceID spaceID) : entityManager(nullptr), entityComponentManager(nullptr), renderingOrder(nullptr), spaceID(spaceID) {}
	BaseGameSpace(const BaseGameSpace&) = delete;
	virtual ~BaseGameSpace() {}

	virtual bool init();
	virtual void release();

	GameSpaceID getID() const { return spaceID; }

	virtual Entity* createEntity() = 0;
	virtual Entity* createEntity(Entity* parent) = 0;
	virtual Entity* createEntity(EntityID parentID) = 0;
	virtual void deleteEntity(Entity* entity) = 0;
	virtual void deleteEntity(EntityID id) = 0;

	EntityComponent* createEntityComponent(EntityComponentType type, Entity* parent);
	EntityComponent* createEntityComponent(EntityComponentType type, EntityID parentID);
	void deleteEntityComponent(EntityComponent* component);
	void deleteEntityComponent(EntityComponentID id);
	void removeEntityComponentsFromEntity(Entity* entity);
	void removeEntityComponentsFromEntity(EntityID id);
 
	//using for deleting children nodes
	void deleteEntityWithoutNode(Entity* entity);
	void deleteEntityWithoutNode(EntityID id);

	EntityManager* getEntityManager() { return entityManager; }
	EntityComponentManager* getEntityComponentManager() { return entityComponentManager; }
};