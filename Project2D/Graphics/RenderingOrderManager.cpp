#include "RenderingOrderManager.h"

void RenderingOrderManager::release() {
	nodes = std::vector<const RenderingOrderNode*>();
	transforms = std::vector<TransformMatrix>();

	nodeIndex = 0;
	reverseNodeIndex = 0;
}

void RenderingOrderManager::clear() {
	nodes.clear();
	transforms.clear();

	nodeIndex = 0;
	reverseNodeIndex = 0;
}

void RenderingOrderManager::reserve(size_t size) {
	nodes.reserve(size);
	transforms.reserve(size);

	nodeIndex = 0;
	reverseNodeIndex = size - 1;
}

void RenderingOrderManager::sortRenderingNodes(size_t beginIndex, size_t endIndex, std::function<bool(const RenderingOrderNode*, const RenderingOrderNode*)> sortPredicate) {
	assert(beginIndex < endIndex&& endIndex < nodes.size());
	std::stable_sort(nodes.begin() + beginIndex, nodes.begin() + endIndex, sortPredicate);
}

void RenderingOrderManager::sortNodes() {
	std::stable_sort(nodes.begin(), nodes.begin() + nodeIndex, [](const RenderingOrderNode* node1, const RenderingOrderNode* node2) {
		return node1->transform->_43 < node2->transform->_43;
	});
}

void RenderingOrderManager::sortRevertedNodes() {
	std::stable_sort(nodes.begin() + nodeIndex, nodes.end(), [](const RenderingOrderNode* node1, const RenderingOrderNode* node2) {
		return node1->transform->_43 > node2->transform->_43;
	});
}

void RenderingOrderManager::pushNode(const RenderingOrderNode& node) {
	assert(nodes.size() > nodeIndex);
	nodes[nodeIndex] = &node;
	++nodeIndex;
}

void RenderingOrderManager::pushRevertedNode(const RenderingOrderNode& node) {
	assert(nodes.size() > reverseNodeIndex && reverseNodeIndex > nodeIndex);
	nodes[reverseNodeIndex] = &node;
	--reverseNodeIndex;
}

TransformMatrix* RenderingOrderManager::pushTransformMatrix(const TransformMatrix& transform) {
	assert(nodes.size() > nodeIndex);
	transforms[nodeIndex] = transform;
	return &transforms[nodeIndex];
}

TransformMatrix* RenderingOrderManager::pushReversedTransformMatrix(const TransformMatrix& transform) {
	assert(nodes.size() > reverseNodeIndex && reverseNodeIndex > nodeIndex);
	transforms[reverseNodeIndex] = transform;
	return &transforms[reverseNodeIndex];
}
