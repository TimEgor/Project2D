#pragma once

#include <Graphics/NodeHandler.h>
#include <Graphics/TransformHandler.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>
#include <MemoryManager/Allocators/PoolAllocator.h>

#include <unordered_map>
#include <mutex>

#define NODES_ALLOCATOR_SIZE 4096

class Scene final {
	typedef PoolAllocatorVector<PoolAllocator> Allocators;

private:
	std::unordered_map<NodeID, NodeHandler> nodes;
	std::unordered_map<TransformID, TransformHandler> transforms;
	Allocators nodeAllocators;
	Allocators transformAllocators;

	Transform& createTransform(NodeID id);
	void deleteTransform(TransformID id);

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