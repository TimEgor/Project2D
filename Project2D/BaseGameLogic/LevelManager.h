#pragma once

#include <BaseGameLogic/GameSpace.h>
#include <ResourceManager/ResourceName.h>

#include <unordered_map>

typedef SceneGameSpace Level;

class LevelManager final {
private:
	std::unordered_map<GameSpaceID, SceneGameSpace> levels;

	Level* currentLevel;
	GameSpaceID nextLevelID;

	LevelManager() : currentLevel(nullptr), nextLevelID(1) {}

public:
	static LevelManager& get();

	Level* getCurrentLevel();

	Level* createLevel();
	//Level& loadLevel(const ResourceName& levelName);

	void changeCurrentLevel(GameSpaceID levelID);
	void changeCurrentLevel(Level* level);
};