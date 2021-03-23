#pragma once

#include <Graphics/Node.h>

class SceneTransform;

class SceneNode final : public Node {
public:
	SceneNode(SceneTransform* transform);

	SceneNode* getParent();
	SceneNode* getChild(NodeID id);

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

	SceneTransform* getTransform();
};