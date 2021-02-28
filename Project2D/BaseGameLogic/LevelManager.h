#pragma once

#include <BaseGameLogic/Level.h>
#include <ResourceManager/ResourceName.h>

#include <unordered_map>

class LevelManager final {
private:
	std::unordered_map<LevelID, Level> levels;

	Level* currentLevel;
	LevelID nextLevelID;

	LevelManager() : currentLevel(nullptr), nextLevelID(1) {}

public:
	static LevelManager& get();

	Level* getCurrentLevel();

	Level* createLevel();
	//Level& loadLevel(const ResourceName& levelName);

	void changeCurrentLevel(LevelID levelID);
	void changeCurrentLevel(Level* level);
};