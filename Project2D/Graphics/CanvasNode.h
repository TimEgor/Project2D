#pragma once

#include <Graphics/Node.h>

class CanvasTransform;

class CanvasNode final : public Node {
public:
	CanvasNode(CanvasTransform* transform);

	CanvasNode* getParent();
	CanvasNode* getChild(NodeID id);

	float getScaleX() const;
	float getScaleY() const;
	void setScaleX(float X);
	void setScaleY(float Y);

	float getRotation() const;
	void setRotation(float rot);

	float getDepth() const;
	void setDepth(float dph);

	float getPivotX() const;
	float getPivotY() const;
	void setPivotX(float X);
	void setPivotY(float Y);

	float getAnchorX() const;
	float getAnchorY() const;
	void setAnchorX(float X);
	void setAnchorY(float Y);

	uint16_t getPositionX() const;
	uint16_t getPositionY() const;
	void setPositionX(uint16_t X);
	void setPositionY(uint16_t Y);

	CanvasTransform* getTransform();
};