#pragma once

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

	LevelID levelID;

public:
	Level(LevelID levelID) : entityManager(nullptr), entityComponentManager(nullptr), scene(nullptr), levelID(levelID) {}
	Level(const Level&) = delete;
	~Level() { release(); }

	bool init();
	void release();

	LevelID getID() const { return levelID; }

	EntityManager& getEntityManager() { return *entityManager; }
	EntityComponentManager& getEntityComponentManager() { return *entityComponentManager; }
	Scene& getScene() { return *scene; }
};