#include "LevelManager.h"

#include <cassert>

LevelManager& LevelManager::get() {
    static LevelManager uniqueLevelManager;
    return uniqueLevelManager;
}

Level* LevelManager::getCurrentLevel() {
    assert(currentLevel && "CurrentLevel is empty !!!");
    return currentLevel;
}

Level* LevelManager::createLevel() {
    auto newValPair = levels.emplace(std::piecewise_construct, std::forward_as_tuple(nextLevelID), std::forward_as_tuple(nextLevelID));
    ++nextLevelID;

    SceneGameSpace* newLevel = &(newValPair.first->second);
    newLevel->init();

    if (!currentLevel) {
        currentLevel = newLevel;
    }

    return newLevel;
}

//Level& LevelManager::loadLevel(const ResourceName& levelName) {
//    // TODO: вставьте здесь оператор return
//}

void LevelManager::changeCurrentLevel(GameSpaceID levelID) {
    auto findLevelIter = levels.find(levelID);
    if (findLevelIter != levels.end()) {
        currentLevel = &(findLevelIter->second);
    }
    else {
        assert(false && "The given level ID isn't valid.");
    }
}

void LevelManager::changeCurrentLevel(Level* level) {
    currentLevel = level;
}
