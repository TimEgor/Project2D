#include "Node.h"

#include <Graphics/NodeHandler.h>
#include <Graphics/NodeManager.h>
#include <Graphics/Transform.h>
#include <BaseGameLogic/LevelManager.h>

#include <cassert>

Node::~Node() {}

void Node::addChild(Node* node) {
	assert(node->parentNode == nullptr);
	node->parentNode = this;

	node->markTransformDirty();

	children.push_back(node);
}

void Node::removeChild(NodeID id) {
	assert(id != 0);

	auto findIter = std::find_if(children.begin(), children.end(),
		[id](const Node* node) -> bool {
			return node->getID() == id;
		});

	if (findIter != children.end()) {
		Node* node = (*findIter);
		node->parentNode = nullptr;

		node->markTransformDirty();

		std::iter_swap(findIter, children.rbegin());
		children.pop_back();
	}
}

void Node::removeChild(Node* node) {
	assert(node);

	auto findIter = std::find(children.begin(), children.end(), node);

	if (findIter != children.end()) {
		node->parentNode = nullptr;

		node->markTransformDirty();

		std::iter_swap(findIter, children.rbegin());
		children.pop_back();
	}
}

void Node::removeAllChildren() {
	children = std::vector<Node*>();
}

void Node::notifyTransformChildren() {
	for (auto child : children) {
		child->markTransformDirty();
	}
}

Node::Node(Transform* transform)
	: parentNode(nullptr), handler(nullptr), transform(transform) {
	transform->markDirty();
}

NodeID Node::getID() const {
	if (handler) {
		return handler->getID();
	}

	return 0;
}

Node* Node::getChild(NodeID id) {
	auto findIter = std::find_if(children.begin(), children.end(),
		[id](const Node* reference) -> bool {
			return reference->getID() == id;
		});

	if (findIter == children.end()) {
		return nullptr;
	}

	return *findIter;
}

bool Node::isTransformDirty() const {
	return transform->isMatrixDirty();
}

void Node::markTransformDirty() {
	transform->markDirty();
	notifyTransformChildren();
}

void Node::updateTransform() {
	if (transform->isMatrixDirty()) {
		if (parentNode) {
			parentNode->updateTransform();
			transform->updateWorldTransformMatrix(parentNode->transform->getWorldTransformMatrix());
		}
		else {
			transform->updateWorldTransformMatrix();
		}
	}
}

const std::vector<Node*>& Node::getChildren() {
	return children;
}

void Node::getChildren(std::vector<Node*> container) {
	container.resize(container.size());
	std::copy(children.begin(), children.end(), container.begin());
}
