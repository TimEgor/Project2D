#pragma once

#include <Graphics/Node.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>

class NodeHandler final {
private:
	Node* node;
	AllocatorID nodeAllocatorID;
	NodeID id;

public:
	NodeHandler(NodeID id, Node* node, size_t nodeAllocatorID)
		: id(id), node(node), nodeAllocatorID(nodeAllocatorID) {
		node->handler = this;
	}

	NodeHandler(const NodeHandler&) = delete;
	NodeHandler(NodeHandler&&) = delete;

	Node* getNode() { return node; }
	NodeID getID() { return id; }

	size_t getNodeAllocatorID() { return nodeAllocatorID; }
};