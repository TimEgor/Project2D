#pragma once

#include <BaseGameLogic/GameSpace.h>
#include <ResourceManager/ResourceName.h>

#include <unordered_map>

class LevelManager final {
private:
	std::unordered_map<GameSpaceID, SceneGameSpace> levels;

	SceneGameSpace* currentLevel;
	GameSpaceID nextLevelID;

	LevelManager() : currentLevel(nullptr), nextLevelID(1) {}

public:
	static LevelManager& get();

	SceneGameSpace* getCurrentLevel();

	SceneGameSpace* createLevel();
	//Level& loadLevel(const ResourceName& levelName);

	void changeCurrentLevel(GameSpaceID levelID);
	void changeCurrentLevel(SceneGameSpace* level);
};