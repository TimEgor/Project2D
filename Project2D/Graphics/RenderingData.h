#pragma once

#include <Graphics/RenderingOrderManager.h>

class Scene;

class RenderingData final {
private:
	SceneRenderingOrderManager sceneOrderManager;
	CanvasRenderingOrderManager canvasOrderManager;
	Scene* scene = nullptr;

public:
	RenderingData(Scene* scene) : scene(scene) {}

	Scene* getScene() { return scene; }
	SceneRenderingOrderManager& getSceneRedneringOrderManager() { return sceneOrderManager; }
	CanvasRenderingOrderManager& getCanvasRedneringOrderManager() { return canvasOrderManager; }
};