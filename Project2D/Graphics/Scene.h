#pragma once

#include <Graphics/NodeHandler.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>

#include <unordered_map>

#define NODES_ALLOCATOR_SIZE 4096

class Scene final {
private:
	std::unordered_map<NodeID, NodeHandler> nodes;
	PoolAllocatorVector nodeAllocators;

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