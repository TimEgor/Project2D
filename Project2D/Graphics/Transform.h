#pragma once

#include <DirectXMath.h>

#include <cstdint>

typedef DirectX::XMFLOAT4X4 TransformMatrix;
typedef uint32_t TransformID;

class Transform final {
private:
	TransformMatrix* worldTransformation;

	float posX, posY;
	float scaleX, scaleY;
	float rotation;
	uint16_t depth;

	bool isDirty;

	void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix);

public:
	Transform(TransformMatrix* worldTransformationMatrix) :
		worldTransformation(worldTransformationMatrix),
		posX(0.0f), posY(0.0f),
		scaleX(1.0f), scaleY(1.0f),
		rotation(0.0f), depth(0),
		isDirty(false) {}

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

	uint16_t getDepth() const { return depth; }
	void setDepth(uint16_t dph);

	bool isMatrixDirty() const { return isDirty; }
	void markDirty();

	TransformMatrix* getWorldTransformMatrix() { return worldTransformation; }
	void updateWorldTransformMatrix(const TransformMatrix* parentTransform);
	void updateWorldTransformMatrix();
};