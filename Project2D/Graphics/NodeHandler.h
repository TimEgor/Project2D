#pragma once

#include <Graphics/Node.h>

class NodeHandler final {
private:
	Node& node;
	size_t nodeAllocatorIndex;
	NodeID id;

public:
	NodeHandler(NodeID id, Node& node, size_t nodeAllocatorIndex)
		: id(id), node(node), nodeAllocatorIndex(nodeAllocatorIndex) {}

	NodeHandler(const NodeHandler&) = delete;
	NodeHandler(NodeHandler&&) = delete;

	Node& getNode() { return node; }
	NodeID getID() { return id; }

	size_t getNodeAllocatorIndex() { return nodeAllocatorIndex; }
};