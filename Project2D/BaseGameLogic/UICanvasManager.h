#pragma once

#include <BaseGameLogic/GameSpace.h>
#include <ResourceManager/ResourceName.h>

#include <unordered_map>

typedef CanvasGameSpace UICanvas;

class UICanvasManager final {
private:
	std::unordered_map<GameSpaceID, CanvasGameSpace> canvases;

	UICanvas* currentCanvas;
	GameSpaceID nextCanvasID;

	UICanvasManager() : currentCanvas(nullptr), nextCanvasID(1) {}

public:
	static UICanvasManager& get();

	UICanvas* getCurrentCanvas();

	UICanvas* createCanvas();

	void changeCurrentCanvas(GameSpaceID canvasID);
	void changeCurrentCanvas(UICanvas* canvas);
};