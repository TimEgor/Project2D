#pragma once

#include <Graphics/RenderingOrder.h>

class Scene;

class RenderingData final {
private:
	Scene* scene = nullptr;
	RenderingOrder* sceneOrder = nullptr;
	RenderingOrder* canvasOrder = nullptr;

public:
	RenderingData() = default;
	
	void setScene(Scene* renderingScene) { scene = renderingScene; }
	Scene* getScene() { return scene; }
	void setSceneRenderingOrder(RenderingOrder* renderingOrder) { sceneOrder = renderingOrder; }
	RenderingOrder* getSceneRedneringOrder() { return sceneOrder; }
	void setCanvasRenderingOrder(RenderingOrder* renderingOrder) { canvasOrder = renderingOrder; }
	RenderingOrder* getCanvasRedneringOrder() { return canvasOrder; }
};