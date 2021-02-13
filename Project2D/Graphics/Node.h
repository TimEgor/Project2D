#pragma once

#include <cstdint>
#include <vector>

class NodeHandler;
class Transform;

typedef uint32_t NodeID;

class Node final {
	friend NodeHandler;

private:
	std::vector<Node*> children;
	Node* parentNode;
	NodeHandler* handler;

	Transform& transform;

public:
	Node(Transform &transform) : parentNode(nullptr), handler(nullptr), transform(transform) {}

	NodeID getID() const;

	void addChild(Node* node);
	void removeChild(NodeID id, bool withDeleting = true);
	void removeChild(Node* node, bool withDeleting = true);
	void removeAllChildren();

	Node* getParent() { return parentNode; }

	Node* getChild(NodeID id);

	Transform& getTransform() { return transform; }

	const std::vector<Node*>& getChildren();
	void getChildren(std::vector<Node*> container);
};