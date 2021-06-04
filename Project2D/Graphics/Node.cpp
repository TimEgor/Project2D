#include "Node.h"

#include <Graphics/NodeHandler.h>
#include <Graphics/Scene.h>
#include <Graphics/Transform.h>
#include <LevelManager/LevelManager.h>

#include <cassert>

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

Node::Node(Transform* transform, Entity* entity)
	: parentNode(nullptr), handler(nullptr), transform(transform), entity(entity) {
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
	if (!transform->isMatrixDirty()) {
		transform->markDirty();
		notifyTransformChildren();
	}
}

void Node::updateTransform() {
	if (transform->isMatrixDirty()) {
		if (parentNode) {
			parentNode->updateTransform();

			transform->updateWorldTransformMatrix(parentNode->transform);
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

SceneTransform* SceneNode::getTransform() {
	return (SceneTransform*)(transform);
}

float CanvasNode::getPivotX() const {
	return ((CanvasTransform*)(transform))->getPivotX();
}

float CanvasNode::getPivotY() const {
	return ((CanvasTransform*)(transform))->getPivotY();
}

void CanvasNode::setPivotX(float X) {
	((CanvasTransform*)(transform))->setPivotX(X);
	notifyTransformChildren();
}

void CanvasNode::setPivotY(float Y) {
	((CanvasTransform*)(transform))->setPivotY(Y);
	notifyTransformChildren();
}

float CanvasNode::getAnchorX() const {
	return ((CanvasTransform*)(transform))->getAnchorX();
}

float CanvasNode::getAnchorY() const {
	return ((CanvasTransform*)(transform))->getAnchorY();
}

void CanvasNode::setAnchorX(float X) {
	((CanvasTransform*)(transform))->setAnchorX(X);
	notifyTransformChildren();
}

void CanvasNode::setAnchorY(float Y) {
	((CanvasTransform*)(transform))->setAnchorY(Y);
	notifyTransformChildren();
}

float CanvasNode::getWidth() const {
	return ((CanvasTransform*)(transform))->getWidth();
}

float CanvasNode::getHeight() const {
	return ((CanvasTransform*)(transform))->getHeight();
}

void CanvasNode::setWidth(float width) {
	((CanvasTransform*)(transform))->setWidth(width);
	notifyTransformChildren();
}

void CanvasNode::setHeight(float height) {
	((CanvasTransform*)(transform))->setHeight(height);
	notifyTransformChildren();
}

CanvasTransform* CanvasNode::getTransform() {
	return (CanvasTransform*)(transform);
}

float Node::getPositionX() const {
	return transform->getPositionX();
}

float Node::getPositionY() const {
	return transform->getPositionY();
}

void Node::setPositionX(float X) {
	transform->setPositionX(X);
	notifyTransformChildren();
}

void Node::setPositionY(float Y) {
	transform->setPositionY(Y);
	notifyTransformChildren();
}

float Node::getScaleX() const {
	return transform->getScaleX();
}

float Node::getScaleY() const {
	return transform->getScaleY();
}

void Node::setScaleX(float X) {
	transform->setScaleX(X);
	notifyTransformChildren();
}

void Node::setScaleY(float Y) {
	transform->setScaleY(Y);
	notifyTransformChildren();
}

float Node::getRotation() const {
	return transform->getRotation();
}

void Node::setRotation(float rot) {
	transform->setRotation(rot);
	notifyTransformChildren();
}

float Node::getDepth() const {
	return transform->getDepth();
}

void Node::setDepth(float dph) {
	transform->setDepth(dph);
	notifyTransformChildren();
}