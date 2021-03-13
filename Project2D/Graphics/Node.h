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

	void notifyTransformChildren();

public:
	Node(Transform& transform);

	NodeHandler* getHandler() { return handler; }
	NodeID getID() const;

	void addChild(Node* node);
	void removeChild(NodeID id);
	void removeChild(Node* node);
	void removeAllChildren();

	Node* getParent() { return parentNode; }

	Node* getChild(NodeID id);

	float getPositionX() const;
	float getPositionY() const;
	void setPositionX(float X);
	void setPositionY(float Y);

	float getScaleX() const;
	float getScaleY() const;
	void setScaleX(float X);
	void setScaleY(float Y);

	float getRotation() const;
	void setRotation(float rot);

	uint16_t getDepth() const;
	void setDepth(uint16_t dph);

	bool isTransformDirty() const;
	void markTransformDirty();

	void updateTransform();

	Transform& getTransform() { return transform; }

	const std::vector<Node*>& getChildren();
	void getChildren(std::vector<Node*> container);
};