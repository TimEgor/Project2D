#pragma once

#include <EntityManager/EntityManagerTypes.h>
#include <EntityManager/EntityComponentType.h>
#include <Graphics/RenderingData.h>

#include <cstdint>

class EntityManager;
class EntityComponentManager;
class Scene;

typedef uint32_t LevelID;

class Level final {
private:
	EntityManager* entityManager;
	EntityComponentManager* entityComponentManager;
	Scene* scene;

	RenderingOrder* renderingOrder;

	LevelID levelID;

	void deleteEntityComponentsFromEntity(Entity* entity);

public:
	Level(LevelID levelID) : entityManager(nullptr), entityComponentManager(nullptr), scene(nullptr), levelID(levelID) {}
	Level(const Level&) = delete;
	~Level() { release(); }

	bool init();
	void release();

	LevelID getID() const { return levelID; }

	Entity* createEntity();
	void createEntity(Entity* parent);
	void createEntity(EntityID parentID);
	void deleteEntity(Entity* entity);
	void deleteEntity(EntityID id);

	EntityComponent* createEntityComponent(EntityComponentType type, Entity* parent);
	EntityComponent* createEntityComponent(EntityComponentType type, EntityID parentID);
	void deleteEntityComponent(EntityComponent* component);
	void deleteEntityComponent(EntityComponentID id);
	void removeEntityComponentsFromEntity(Entity* entity);
	void removeEntityComponentsFromEntity(EntityID id);

	//using for deleting children nodes
	void deleteEntityWithoutNode(Entity* entity);
	void deleteEntityWithoutNode(EntityID id);

	RenderingData getRenderingData();

	EntityManager* getEntityManager() { return entityManager; }
	EntityComponentManager* getEntityComponentManager() { return entityComponentManager; }
	Scene* getScene() { return scene; }
};