#include "LogicManager.h"

#include <LogicManager/CPP_Logic.h>

LogicManager& LogicManager::get() {
    static LogicManager uniqueLogicManager;
    return uniqueLogicManager;
}

bool LogicManager::init() {
#ifdef CPP_LOGIC
    cppLogic = new CPP_Logic();
    if (!cppLogic || !cppLogic->init()) {
        return false;
    }
#endif // CPP_LOGIC

    return true;
}

void LogicManager::release() {
    if (cppLogic) {
        cppLogic->release();
        delete cppLogic;
        cppLogic = nullptr;
    }
}
