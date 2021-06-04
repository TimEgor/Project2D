#pragma once

#include <Graphics/SceneTypes.h>

#include <vector>

class NodeHandler;
class Entity;

class Node {
	friend NodeHandler;

protected:
	std::vector<Node*> children;
	Node* parentNode;
	NodeHandler* handler;
	Entity* entity;

	Transform* transform;

	void notifyTransformChildren();

public:
	Node(Transform* transform, Entity* entity);
	virtual ~Node() {}

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

	float getDepth() const;
	void setDepth(float dph);

	Transform* getTransform() { return transform; }

	Entity* getEntity() { return entity; }

	const std::vector<Node*>& getChildren();
	void getChildren(std::vector<Node*> container);

	virtual NodeType getNodeType() const = 0;
};

class SceneNode final : public Node {
public:
	SceneNode(SceneTransform* transform, Entity* entity)
		: Node((Transform*)(transform), entity) {}

	SceneTransform* getTransform();

	virtual NodeType getNodeType() const override { return SceneNodeType; }
};

class CanvasNode final : public Node {
public:
	CanvasNode(CanvasTransform* transform, Entity* entity)
		: Node((Transform*)(transform), entity) {}

	float getPivotX() const;
	float getPivotY() const;
	void setPivotX(float X);
	void setPivotY(float Y);

	float getAnchorX() const;
	float getAnchorY() const;
	void setAnchorX(float X);
	void setAnchorY(float Y);

	float getWidth() const;
	float getHeight() const;
	void setWidth(float width);
	void setHeight(float height);

	CanvasTransform* getTransform();

	virtual NodeType getNodeType() const override { return CanvasNodeType; }
};