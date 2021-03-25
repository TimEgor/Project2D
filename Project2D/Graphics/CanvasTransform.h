#pragma once

#include <Graphics/Transform.h>

class CanvasTransform final : public Transform {
private:
	float scaleX, scaleY;
	float rotation;
	float depth;
	float pivotX, pivotY;
	float anchorX, anchorY;
	float posX, posY;
	float width, height;

	virtual void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const Transform* parentTransform) override;
	virtual void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix) override;

public:
	CanvasTransform(TransformMatrix* worldTransformationMatrix) : Transform(worldTransformationMatrix),
		scaleX(1.0f), scaleY(1.0f),
		rotation(0.0f), depth(0.0f),
		pivotX(0.5f), pivotY(0.5f),
		anchorX(0.5f), anchorY(0.5f),
		posX(0.0f), posY(0.0f),
		width(100.0f), height(100.0f) {}

	float getScaleX() const { return scaleX; }
	float getScaleY() const { return scaleY; }
	void setScaleX(float X);
	void setScaleY(float Y);

	float getRotation() const { return rotation; }
	void setRotation(float rot);

	float getDepth() const { return depth; }
	void setDepth(float dph);

	float getPivotX() const { return pivotX; }
	float getPivotY() const { return pivotY; }
	void setPivotX(float X);
	void setPivotY(float Y);

	float getAnchorX() const { return anchorX; }
	float getAnchorY() const { return anchorY; }
	void setAnchorX(float X);
	void setAnchorY(float Y);

	float getPositionX() const { return posX; }
	float getPositionY() const { return posY; }
	void setPositionX(float X);
	void setPositionY(float Y);

	float getWidth() const { return width; }
	float getHeight() const { return height; }
	void setWidth(float width);
	void setHeight(float height);
};