#include "LevelManager.h"

#include <cassert>

LevelManager& LevelManager::get() {
    static LevelManager uniqueLevelManager;
    return uniqueLevelManager;
}

Level& LevelManager::getCurrentLevel() {
    assert(currentLevel && "CurrentLevel is empty !!!");
    return *currentLevel;
}

Level& LevelManager::createLevel() {
    auto newValPair = levels.emplace(std::piecewise_construct, std::forward_as_tuple(nextLevelID), std::forward_as_tuple(nextLevelID));
    ++nextLevelID;

    if (!currentLevel) {
        currentLevel = &newValPair.first->second;
    }

    return newValPair.first->second;
}

//Level& LevelManager::loadLevel(const ResourceName& levelName) {
//    // TODO: вставьте здесь оператор return
//}

void LevelManager::changeCurrentLevel(LevelID levelID) {
}

void LevelManager::changeCurrentLevel(const Level& level) {
}
