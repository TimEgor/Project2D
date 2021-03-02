#pragma once

#include <Graphics/RenderingOrder.h>

class Scene;

class RenderingData final {
private:
	Scene* scene;
	RenderingOrder* order;

public:
	RenderingData() : scene(nullptr), order(nullptr) {}
	
	void setScene(Scene* renderingScene) { scene = renderingScene; }
	Scene* getScene() { return scene; }
	void setRenderingOrder(RenderingOrder* renderingOrder) { order = renderingOrder; }
	RenderingOrder* getRedneringOrder() { return order; }
};