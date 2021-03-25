#pragma once

#include <Graphics/Transform.h>

class SceneTransform final : public Transform {
private:
	float posX, posY;
	float scaleX, scaleY;
	float rotation;
	float depth;

	void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const Transform* parentTransform) override;
	void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix) override;

public:
	SceneTransform(TransformMatrix* worldTransformationMatrix) : Transform(worldTransformationMatrix),
		posX(0.0f), posY(0.0f),
		scaleX(1.0f), scaleY(1.0f),
		rotation(0.0f), depth(0.0f) {}

	float getPositionX() const { return posX; }
	float getPositionY() const { return posY; }
	void setPositionX(float X);
	void setPositionY(float Y);

	float getScaleX() const { return scaleX; }
	float getScaleY() const { return scaleY; }
	void setScaleX(float X);
	void setScaleY(float Y);

	float getRotation() const { return rotation; }
	void setRotation(float rot);

	float getDepth() const { return depth; }
	void setDepth(float dph);
};