#pragma once

#include <Graphics/RenderingNode.h>
#include <Graphics/RenderingOrder.h>

#include <functional>

class RenderingNodeManager final {
protected:
	std::unordered_map<RenderingNodeType, RenderingOrder> containers;
	std::vector<const RenderingNode&> nodes;
	std::vector<TransformMatrix> transforms;

public:
	RenderingNodeManager() = default;

	void release();
	void clear();

	void reserve(size_t size);

	template <typename... Args>
	void pushNode(RenderingNodeType type, const TransformMatrix& transform, Args... args);
};