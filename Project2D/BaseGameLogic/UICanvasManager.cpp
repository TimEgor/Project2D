#include "UICanvasManager.h"

#include <cassert>

UICanvasManager& UICanvasManager::get() {
    static UICanvasManager uniqueUICanvasManager;
    return uniqueUICanvasManager;
}

UICanvas* UICanvasManager::getCurrentCanvas() {
    assert(currentCanvas && "CurrentCanvas is empty !!!");
    return currentCanvas;
}

UICanvas* UICanvasManager::createCanvas() {
    auto newValPair = canvases.emplace(std::piecewise_construct, std::forward_as_tuple(nextCanvasID), std::forward_as_tuple(nextCanvasID));
    ++nextCanvasID;

    CanvasGameSpace* newCanvas = &(newValPair.first->second);
    newCanvas->init();

    //initing root node
    newCanvas->createEntity();
    CanvasNode* rootNode = newCanvas->getNodeManager()->getNode(1);
    rootNode->setWidth(800.0f);
    rootNode->setHeight(800.0f);


    if (!currentCanvas) {
        currentCanvas = newCanvas;
    }

    return newCanvas;
}

void UICanvasManager::changeCurrentCanvas(GameSpaceID canvasID) {
    auto findCanvasIter = canvases.find(canvasID);
    if (findCanvasIter != canvases.end()) {
        currentCanvas = &(findCanvasIter->second);
    }
    else {
        assert(false && "The given canvas ID isn't valid.");
    }
}

void UICanvasManager::changeCurrentCanvas(UICanvas* canvas) {
    currentCanvas = canvas;
}
