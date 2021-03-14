#include "Transform.h"

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

void Transform::calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix) {
	DirectX::XMMATRIX translatingMatrix = DirectX::XMMatrixTranslation(posX, posY, (float)(depth));
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation));
	DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixScaling(scaleX, scaleY, 1.0f);

	rotationMatrix = DirectX::XMMatrixMultiply(scalingMatrix, rotationMatrix);
	matrix = DirectX::XMMatrixMultiply(rotationMatrix, translatingMatrix);
}

void Transform::updateWorldTransformMatrix(const TransformMatrix* parentTransform) {
	assert(parentTransform);

	DirectX::XMMATRIX transformMatrix;
	calculateLocalTransfomMatrix(transformMatrix);

	DirectX::XMMATRIX parent = DirectX::XMLoadFloat4x4(parentTransform);

	transformMatrix = DirectX::XMMatrixMultiply(transformMatrix, parent);
	DirectX::XMStoreFloat4x4(worldTransformation, transformMatrix);

	isDirty = false;
}

void Transform::updateWorldTransformMatrix() {
	DirectX::XMMATRIX transformMatrix;
	calculateLocalTransfomMatrix(transformMatrix);
	DirectX::XMStoreFloat4x4(worldTransformation, transformMatrix);

	isDirty = false;
}
