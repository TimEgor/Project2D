#pragma once

#include <Graphics/NodeHandler.h>
#include <BaseGameLogic/BaseGameSpace.h>
#include <Graphics/TransformHandler.h>
#include <Graphics/CanvasNode.h>
#include <Graphics/SceneNode.h>
#include <Graphics/CanvasTransform.h>
#include <Graphics/SceneTransform.h>
#include <MemoryManager/MemoryManager.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>
#include <MemoryManager/Allocators/PoolAllocator.h>

#include <unordered_map>

#define NODES_ALLOCATOR_SIZE 4096

template <typename NodeT, typename TransformT>
class NodeManager final {
private:
	typedef PoolAllocatorVector<PoolAllocator> Allocators;

public:
	typedef NodeT NodeType;
	typedef TransformT TransformType;

private:
	std::unordered_map<NodeID, NodeHandler> nodes;
	std::unordered_map<TransformID, TransformHandler> transforms;
	Allocators nodeAllocators;
	Allocators transformAllocators;
	Allocators worldTransformsMatrixAllocators;

	BaseGameSpace* gameSpace;

	size_t nodesAllocatorSize;

	TransformType* createTransform(NodeID id);
	void deleteTransform(TransformID id);

	void deleteChildrenNodes(NodeType* node);

public:
	NodeManager() = default;
	~NodeManager() { release(); }

	bool init(BaseGameSpace* gameSpace, size_t allocatorSize = NODES_ALLOCATOR_SIZE);
	void release();

	NodeType* createNode(NodeID id);
	NodeType* createNode(NodeID id, NodeID parentID);
	NodeType* createNode(NodeID id, NodeType* parent);
	void deleteNode(NodeID id);
	void deleteNode(NodeType* node);

	NodeType* getNode(NodeID id);
	TransformType* getTransform(TransformID id);
};

typedef NodeManager<CanvasNode, CanvasTransform> CanvasNodeManager;
typedef NodeManager<SceneNode, SceneTransform> SceneNodeManager;

template<typename NodeType, typename TransformType>
inline bool NodeManager<NodeType, TransformType>::init(BaseGameSpace* space, size_t allocatorSize) {
	gameSpace = space;

	nodesAllocatorSize = allocatorSize;

	nodes.reserve(nodesAllocatorSize);
	if (!nodeAllocators.init(MemoryManager::get().getDefaultHeap(), sizeof(NodeType), nodesAllocatorSize)) {
		release();
		return false;
	}

	if (!transformAllocators.init(MemoryManager::get().getDefaultHeap(), sizeof(TransformType), nodesAllocatorSize)) {
		release();
		return false;
	}

	if (!worldTransformsMatrixAllocators.init(MemoryManager::get().getDefaultHeap(), sizeof(TransformMatrix), nodesAllocatorSize)) {
		release();
		return false;
	}

	return true;
}

template<typename NodeType, typename TransformType>
inline void NodeManager<NodeType, TransformType>::release() {
	nodes = std::unordered_map<NodeID, NodeHandler>();
	nodeAllocators.release();
}

template<typename NodeType, typename TransformType>
inline TransformType* NodeManager<NodeType, TransformType>::createTransform(NodeID id) {
	size_t oldAllocatorCount = transformAllocators.size();

	Allocators::AllocationInfo transformAllocationInfo = transformAllocators.allocate();
	Allocators::AllocationInfo matrixAllocationInfo = worldTransformsMatrixAllocators.allocate();
	TransformMatrix* matrix = new (matrixAllocationInfo.allocationAddress) TransformMatrix();
	TransformType* newTransform = new (transformAllocationInfo.allocationAddress) TransformType(matrix);

	size_t newAllocatorCount = transformAllocators.size();
	if (oldAllocatorCount < newAllocatorCount) {
		transforms.reserve(newAllocatorCount * nodesAllocatorSize);
	}

	transforms.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, newTransform, transformAllocationInfo.allocatorID));

	return newTransform;
}

template<typename NodeType, typename TransformType>
inline void NodeManager<NodeType, TransformType>::deleteTransform(TransformID id) {
	auto findIter = transforms.find(id);
	if (findIter != transforms.end()) {
		TransformHandler& handler = findIter->second;
		TransformType* transform = (TransformType*)(handler.getTransform());

		worldTransformsMatrixAllocators.deallocate(handler.getTransformAllocatorID(), transform->getWorldTransformMatrix());
		transformAllocators.deallocate(handler.getTransformAllocatorID(), transform);

		transforms.erase(findIter);
	}
}

template<typename NodeType, typename TransformType>
inline void NodeManager<NodeType, TransformType>::deleteChildrenNodes(NodeType* node) {
	auto childrens = node->getChildren();
	for (auto child : childrens) {
		NodeID childID = child->getID();
		gameSpace->deleteEntityWithoutNode(childID);
		deleteNode(childID);
	}
}

template<typename NodeType, typename TransformType>
inline NodeType* NodeManager<NodeType, TransformType>::createNode(NodeID id) {
	size_t oldAllocatorCount = nodeAllocators.size();

	Allocators::AllocationInfo allocationInfo = nodeAllocators.allocate();

	TransformType* transform = createTransform(id);

	NodeType* newNode = new (allocationInfo.allocationAddress) NodeType(transform);

	size_t newAllocatorCount = nodeAllocators.size();
	if (oldAllocatorCount < newAllocatorCount) {
		nodes.reserve(newAllocatorCount * nodesAllocatorSize);
	}

	nodes.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, newNode, allocationInfo.allocatorID));

	return newNode;
}

template<typename NodeType, typename TransformType>
inline NodeType* NodeManager<NodeType, TransformType>::createNode(NodeID id, NodeID parentID) {
	NodeType* newNode = nullptr;

	auto nodeIter = nodes.find(parentID);
	if (nodeIter != nodes.end()) {
		NodeHandler& handler = nodeIter->second;
		NodeType* parentNode = (NodeType*)(handler.getNode());

		newNode = createNode(id);

		parentNode->addChild(newNode);
	}

	return newNode;
}

template<typename NodeType, typename TransformType>
inline NodeType* NodeManager<NodeType, TransformType>::createNode(NodeID id, NodeType* parent) {
	return createNode(id, parent->getID());
}

template<typename NodeType, typename TransformType>
inline void NodeManager<NodeType, TransformType>::deleteNode(NodeID id) {
	auto nodeIter = nodes.find(id);
	if (nodeIter != nodes.end()) {
		NodeHandler& handler = nodeIter->second;
		NodeType* node = (NodeType*)(handler.getNode());

		deleteTransform(nodeIter->first);
		deleteChildrenNodes(node);

		nodeAllocators.deallocate(handler.getNodeAllocatorID(), node);

		nodes.erase(nodeIter);
	}
}

template<typename NodeType, typename TransformType>
inline void NodeManager<NodeType, TransformType>::deleteNode(NodeType* node) {
	deleteNode(node->getID());
}

template<typename NodeType, typename TransformType>
inline NodeType* NodeManager<NodeType, TransformType>::getNode(NodeID id) {
	auto findIter = nodes.find(id);
	if (findIter == nodes.end()) {
		return nullptr;
	}

	return (NodeType*)(findIter->second.getNode());
}

template<typename NodeType, typename TransformType>
inline TransformType* NodeManager<NodeType, TransformType>::getTransform(TransformID id) {
	auto findIter = transforms.find(id);
	if (findIter == transforms.end()) {
		return nullptr;
	}

	return (TransformType*)(findIter->second.getTransform());
}
