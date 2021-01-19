#include "Node.h"

#include <Graphics/NodeHandler.h>
#include <Graphics/Scene.h>
#include <BaseGameLogic/LevelManager.h>

#include <cassert>

void Node::addChild(Node* node) {
	assert(node->parentNode == nullptr);
	node->parentNode = this;

	children.push_back(node);
}

void Node::removeChild(NodeID id, bool withDeleting) {
	assert(id != 0);

	auto findIter = std::find_if(children.begin(), children.end(),
		[id](const Node* node) -> bool {
			return node->getID() == id;
		});

	if (findIter != children.end()) {
		Node* node = (*findIter);
		node->parentNode = nullptr;

		if (node->handler && withDeleting) {
			LevelManager::get().getCurrentLevel().getScene().deleteNode(node);
		}

		std::iter_swap(findIter, children.rbegin());
		children.pop_back();
	}
}

void Node::removeChild(Node* node, bool withDeleting) {
	assert(node);

	auto findIter = std::find(children.begin(), children.end(), node);

	if (findIter != children.end()) {
		node->parentNode = nullptr;

		if (node->handler && withDeleting) {
			LevelManager::get().getCurrentLevel().getScene().deleteNode(node);
		}

		std::iter_swap(findIter, children.rbegin());
		children.pop_back();
	}
}

void Node::removeAllChildren() {
	for (auto* child : children) {
		if (child->handler) {
			LevelManager::get().getCurrentLevel().getScene().deleteNode(child);
		}
	}

	children = std::vector<Node*>();
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

const std::vector<Node*>& Node::getChildren() {
	return children;
}

void Node::getChildren(std::vector<Node*> container) {
	container.resize(container.size());
	std::copy(children.begin(), children.end(), container.begin());
}
