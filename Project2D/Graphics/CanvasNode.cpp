#include "CanvasNode.h"

#include <Graphics/CanvasTransform.h>

CanvasNode::CanvasNode(CanvasTransform* transform)
    : Node(transform) {}

CanvasNode* CanvasNode::getParent() {
    return (CanvasNode*)(Node::getParent());
}

CanvasNode* CanvasNode::getChild(NodeID id) {
    return (CanvasNode*)(Node::getChild(id));
}

float CanvasNode::getScaleX() const {
    return ((CanvasTransform*)(transform))->getScaleX();
}

float CanvasNode::getScaleY() const {
    return ((CanvasTransform*)(transform))->getScaleY();
}

void CanvasNode::setScaleX(float X) {
    ((CanvasTransform*)(transform))->setScaleX(X);
    notifyTransformChildren();
}

void CanvasNode::setScaleY(float Y) {
    ((CanvasTransform*)(transform))->setScaleY(Y);
    notifyTransformChildren();
}

float CanvasNode::getRotation() const {
    return ((CanvasTransform*)(transform))->getRotation();
}

void CanvasNode::setRotation(float rot) {
    ((CanvasTransform*)(transform))->setRotation(rot);
    notifyTransformChildren();
}

float CanvasNode::getDepth() const {
    return ((CanvasTransform*)(transform))->getDepth();
}

void CanvasNode::setDepth(float dph) {
    ((CanvasTransform*)(transform))->setDepth(dph);
    notifyTransformChildren();
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

float CanvasNode::getPositionX() const {
    return ((CanvasTransform*)(transform))->getPositionX();
}

float CanvasNode::getPositionY() const {
    return ((CanvasTransform*)(transform))->getPositionY();
}

void CanvasNode::setPositionX(float X) {
    ((CanvasTransform*)(transform))->setPositionX(X);
    notifyTransformChildren();
}

void CanvasNode::setPositionY(float Y) {
    ((CanvasTransform*)(transform))->setPositionY(Y);
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
