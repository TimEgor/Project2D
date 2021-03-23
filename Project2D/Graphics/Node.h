#pragma once

#include <cstdint>
#include <vector>

class NodeHandler;
class Transform;

typedef uint32_t NodeID;

class Node {
	friend NodeHandler;

protected:
	std::vector<Node*> children;
	Node* parentNode;
	NodeHandler* handler;

	Transform* transform;

	void notifyTransformChildren();

public:
	Node(Transform* transform);
	virtual ~Node() = 0;

	NodeHandler* getHandler() { return handler; }
	NodeID getID() const;

	void addChild(Node* node);
	void removeChild(NodeID id);
	void removeChild(Node* node);
	void removeAllChildren();

	Node* getParent() { return parentNode; }

	Node* getChild(NodeID id);

	bool isTransformDirty() const;
	void markTransformDirty();

	void updateTransform();

	Transform* getTransform() { return transform; }

	const std::vector<Node*>& getChildren();
	void getChildren(std::vector<Node*> container);
};