#pragma once

#include <cstdint>
#include <vector>

class NodeHandler;

typedef uint32_t NodeID;

class Node final {
private:
	std::vector<Node*> children;
	Node* parentNode;
	NodeHandler* handler;

public:
	Node() : parentNode(nullptr), handler(nullptr) {}

	NodeID getID() const;

	void addChild(Node* node);
	void removeChild(NodeID id, bool withDeleting = true);
	void removeChild(Node* node, bool withDeleting = true);
	void removeAllChildren();

	Node* getParent() { return parentNode; }

	Node* getChild(NodeID id);

	const std::vector<Node*>& getChildren();
	void getChildren(std::vector<Node*> container);
};