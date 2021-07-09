#pragma once

#include <EntityManager/EntityManager.h>
#include <EntityManager/EntityComponentManager.h>
#include <Graphics/SceneTypes.h>

#include <cstdint>

class EntityManager;
class EntityComponentManager;
class CppGameLogicClassGroupManager;

typedef uint32_t LevelID;

class Level final {
private:
	EntityManager* entityManager;
	EntityComponentManager* entityComponentManager;
	Scene* scene;

	CppGameLogicClassGroupManager* cppGameLogicClassGroupManager;

	LevelID levelID;

	void deleteEntityComponentsFromEntity(Entity* entity);

public:
	Level(LevelID levelID) : entityManager(nullptr), entityComponentManager(nullptr), scene(nullptr), levelID(levelID) {}
	Level(const Level&) = delete;
	~Level() { release(); }

	bool init();
	void release();

	LevelID getID() const { return levelID; }

	void update(float deltaTime);

	Entity* createSceneEntity();
	Entity* createSceneEntity(Entity* parent);
	Entity* createSceneEntity(EntityID parentID);
	Entity* createCanvasEntity();
	Entity* createCanvasEntity(Entity* parent);
	Entity* createCanvasEntity(EntityID parentID);
	void deleteEntity(Entity* entity);
	void deleteEntity(EntityID id);

	template <typename ComponentType, typename... Args>
	ComponentType* createEntityComponent(Entity* parent, Args... args);
	template <typename ComponentType, typename... Args>
	ComponentType* createEntityComponent(EntityID parentID, Args... args);

	void deleteEntityComponent(EntityComponent* component);
	void deleteEntityComponent(EntityComponentID id);
	void removeEntityComponentsFromEntity(Entity* entity);
	void removeEntityComponentsFromEntity(EntityID id);

	//using for deleting children nodes
	void deleteEntityWithoutNode(Entity* entity);
	void deleteEntityWithoutNode(EntityID id);

	inline EntityManager* getEntityManager() { return entityManager; }
	inline EntityComponentManager* getEntityComponentManager() { return entityComponentManager; }
	inline Scene* getScene() { return scene; }
	inline CppGameLogicClassGroupManager* getCppGameLogicClassGroupManager() { return cppGameLogicClassGroupManager; }
};

template<typename ComponentType, typename... Args>
inline ComponentType* Level::createEntityComponent(Entity* parent, Args... args) {
	assert(parent);

	ComponentType* component = entityComponentManager->createComponent<ComponentType>(args...);

	parent->addComponent(component->getID());
	component->getHandler()->setParent(parent);

	return (ComponentType*)(component);
}

template<typename ComponentType, typename... Args>
inline ComponentType* Level::createEntityComponent(EntityID parentID, Args... args) {
	Entity* entity = entityManager->getEntity(parentID);
	return createEntityComponent<ComponentType>(entity, args...);
}
