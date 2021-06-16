#pragma once

#include <Graphics/RenderingOrderNode.h>
#include <Graphics/RenderingOrder.h>

#include <functional>

enum RenderingOrderManagerType {
	SceneRedneringOrderManagerType,
	CanvasRedneringOrderManagerType
};

class RenderingOrderManager {
protected:
	std::vector<const RenderingOrderNode*> nodes;
	std::vector<TransformMatrix> transforms;

	size_t nodeIndex;
	size_t reverseNodeIndex;

public:
	RenderingOrderManager() = default;
	virtual ~RenderingOrderManager() {}

	void release();
	void clear();

	void reserve(size_t size);

	size_t getForwardNodeIndex() const { return nodeIndex; }
	size_t getReverseNodeIndex() const { return reverseNodeIndex; }

	void sortRenderingNodes(size_t beginIndex, size_t endIndex, std::function<bool(const RenderingOrderNode*, const RenderingOrderNode*)> sortPredicate);

	void sortNodes();
	void sortRevertedNodes();

	void pushNode(const RenderingOrderNode& node);
	void pushRevertedNode(const RenderingOrderNode& node);

	TransformMatrix* pushTransformMatrix(const TransformMatrix& transform);
	TransformMatrix* pushReversedTransformMatrix(const TransformMatrix& transform);

	virtual RenderingOrderManagerType getType() const = 0;
};

class SceneRenderingOrderManager final : public RenderingOrderManager {
private:
	RenderingOrder<SpriteRenderingOrderNode> spriteRenderingOrder;

public:
	SceneRenderingOrderManager() = default;

	RenderingOrder<SpriteRenderingOrderNode>& getSpriteRenderingOrder() { return spriteRenderingOrder; }

	virtual RenderingOrderManagerType getType() const { return SceneRedneringOrderManagerType; }
};

class CanvasRenderingOrderManager final : public RenderingOrderManager {
private:

public:
	CanvasRenderingOrderManager() = default;

	virtual RenderingOrderManagerType getType() const { return CanvasRedneringOrderManagerType; }
};