#include "SceneNode.h"

#include <Graphics/SceneTransform.h>

SceneNode::SceneNode(SceneTransform* transform)
	: Node(transform) {}

SceneNode* SceneNode::getParent() {
	return (SceneNode*)(Node::getParent());
}

SceneNode* SceneNode::getChild(NodeID id) {
	return (SceneNode*)(Node::getChild(id));
}

float SceneNode::getPositionX() const {
	return ((SceneTransform*)(transform))->getPositionX();
}

float SceneNode::getPositionY() const {
	return ((SceneTransform*)(transform))->getPositionY();
}

void SceneNode::setPositionX(float X) {
	((SceneTransform*)(transform))->setPositionX(X);
	notifyTransformChildren();
}

void SceneNode::setPositionY(float Y) {
	((SceneTransform*)(transform))->setPositionY(Y);
	notifyTransformChildren();
}

float SceneNode::getScaleX() const {
	return ((SceneTransform*)(transform))->getScaleX();
}

float SceneNode::getScaleY() const {
	return ((SceneTransform*)(transform))->getScaleY();
}

void SceneNode::setScaleX(float X) {
	((SceneTransform*)(transform))->setScaleX(X);
	notifyTransformChildren();
}

void SceneNode::setScaleY(float Y) {
	((SceneTransform*)(transform))->setScaleY(Y);
	notifyTransformChildren();
}

float SceneNode::getRotation() const {
	return ((SceneTransform*)(transform))->getRotation();
}

void SceneNode::setRotation(float rot) {
	((SceneTransform*)(transform))->setRotation(rot);
	notifyTransformChildren();
}

float SceneNode::getDepth() const {
	return ((SceneTransform*)(transform))->getDepth();
}

void SceneNode::setDepth(float dph) {
	((SceneTransform*)(transform))->setDepth(dph);
}

SceneTransform* SceneNode::getTransform() {
	return (SceneTransform*)(transform);
}
