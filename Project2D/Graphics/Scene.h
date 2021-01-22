#pragma once

#include <Graphics/NodeHandler.h>
#include <Graphics/TransformHandler.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>

#include <unordered_map>
#include <mutex>

#define NODES_ALLOCATOR_SIZE 4096

class Scene final {
private:
	std::mutex graphicMtx;
	std::unordered_map<NodeID, NodeHandler> nodes;
	std::unordered_map<TransformID, TransformHandler> transforms;
	std::unordered_map<TransformID, TransformHandler> graphicTransforms;
	PoolAllocatorVector nodeAllocators;
	PoolAllocatorVector transformAllocators;
	PoolAllocatorVector graphicTransformAllocators;

	Transform& createTransform(NodeID id);
	TransformHandler& createGraphicTransform(NodeID id);

	void deleteTransform(TransformID id);
	void deleteGraphicalTransform(TransformID id);

public:
	Scene() = default;
	~Scene() { release(); }

	bool init();
	void release();

	Node* createNode(NodeID id);
	void deleteNode(NodeID id);
	void deleteNode(Node* node);

	Node* getNode(NodeID id);
};