#include "Transform.h"

#include <DirectXMath.h>

void Transform::setPositionX(float X) {
	posX = X;
	isDirty = true;
}

void Transform::setPositionY(float Y) {
	posY = Y;
	isDirty = true;
}

void Transform::setScaleX(float X) {
	scaleX = X;
	isDirty = true;
}

void Transform::setScaleY(float Y) {
	scaleY = Y;
	isDirty = true;
}

void Transform::setRotation(float rot) {
	rotation = rot;
	isDirty = true;
}

void Transform::setDepth(uint16_t dph) {
	depth = dph;
	isDirty = true;
}

void Transform::markDirty() {
	isDirty = true;
}

void Transform::updateWorldTransformMatrix(const TransformMatrix* parentTransform) {
	assert(parentTransform);

	TransformMatrix translatingMatrix = DirectX::XMMatrixTranslation(posX, posY, (float)(depth));
	TransformMatrix rotationMatrix = DirectX::XMMatrixRotationZ(rotation);
	TransformMatrix scalingMatrix = DirectX::XMMatrixScaling(scaleX, scaleY, 1.0f);

	rotationMatrix = DirectX::XMMatrixMultiply(scalingMatrix, rotationMatrix);
	translatingMatrix = DirectX::XMMatrixMultiply(rotationMatrix, translatingMatrix);

	*worldTransformation = DirectX::XMMatrixMultiply(*parentTransform, translatingMatrix);

	isDirty = false;
}

void Transform::updateWorldTransformMatrix() {
	*worldTransformation = DirectX::XMMatrixTranslation(posX, posY, (float)(depth));
	TransformMatrix rotationMatrix = DirectX::XMMatrixRotationZ(rotation);
	TransformMatrix scalingMatrix = DirectX::XMMatrixScaling(scaleX, scaleY, 1.0f);

	rotationMatrix = DirectX::XMMatrixMultiply(scalingMatrix, rotationMatrix);
	*worldTransformation = DirectX::XMMatrixMultiply(rotationMatrix, *worldTransformation);

	isDirty = false;
}
