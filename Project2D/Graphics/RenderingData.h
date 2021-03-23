#pragma once

#include <Graphics/RenderingOrder.h>
#include <Graphics/NodeManager.h>

template <typename NodeManagerType>
class NodeManagerRenderingData final {
private:
	RenderingOrder* order;
	NodeManagerType* nodeManager;

public:
	NodeManagerRenderingData() : nodeManager(nullptr) {}

	void setRenderingOrder(RenderingOrder* renderingOrder) { order = renderingOrder; }
	RenderingOrder* getRedneringOrder() { return order; }
	NodeManagerType* getNodeManager() { return nodeManager; }
	void setNodeManager(NodeManagerType* manager) { nodeManager = manager; }
};

typedef NodeManagerRenderingData<CanvasNodeManager> CanvasSpaceRenderingData;
typedef NodeManagerRenderingData<SceneNodeManager> SceneSpaceRenderingData;