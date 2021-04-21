#include "Transform.h"

#include <cassert>

void SceneTransform::calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const SceneTransform* parentTransform) {
	calculateLocalTransfomMatrix(matrix);
}

void SceneTransform::calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const CanvasTransform* parentTransform) {
	calculateLocalTransfomMatrix(matrix);
}

void SceneTransform::calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix) {
	DirectX::XMMATRIX translatingMatrix = DirectX::XMMatrixTranslation(posX, posY, depth);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation));
	DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixScaling(scaleX, scaleY, 1.0f);

	rotationMatrix = DirectX::XMMatrixMultiply(scalingMatrix, rotationMatrix);
	matrix = DirectX::XMMatrixMultiply(rotationMatrix, translatingMatrix);
}

void SceneTransform::setPositionX(float X) {
	posX = X;
	isDirty = true;
}

void SceneTransform::setPositionY(float Y) {
	posY = Y;
	isDirty = true;
}

void SceneTransform::setScaleX(float X) {
	scaleX = X;
	isDirty = true;
}

void SceneTransform::setScaleY(float Y) {
	scaleY = Y;
	isDirty = true;
}

void SceneTransform::setRotation(float rot) {
	rotation = rot;
	isDirty = true;
}

void SceneTransform::setDepth(float dph) {
	depth = dph;
	isDirty = true;
}

void CanvasTransform::calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const SceneTransform* parentTransform) {
	float currentPosX = (width * scaleX) * (0.5f - pivotX);
	float currentPosY = (height * scaleY) * (0.5f - pivotY);
	currentPosX += posX;
	currentPosY += posY;

	DirectX::XMMATRIX translatingMatrix = DirectX::XMMatrixTranslation(currentPosX, currentPosY, depth);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation));
	DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixScaling(scaleX, scaleY, 1.0f);

	rotationMatrix = DirectX::XMMatrixMultiply(scalingMatrix, rotationMatrix);
	matrix = DirectX::XMMatrixMultiply(rotationMatrix, translatingMatrix);
}

void CanvasTransform::calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const CanvasTransform* parentTransform) {
	float currentPosX = (parentTransform->width * parentTransform->scaleX) * anchorX;
	float currentPosY = (parentTransform->height * parentTransform->scaleY) * anchorY;
	currentPosX += (width * scaleX) * (0.5f - pivotX);
	currentPosY += (height * scaleY) * (0.5f - pivotY);
	currentPosX += posX;
	currentPosY += posY;

	DirectX::XMMATRIX translatingMatrix = DirectX::XMMatrixTranslation(currentPosX, currentPosY, depth);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation));
	DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixScaling(scaleX, scaleY, 1.0f);

	rotationMatrix = DirectX::XMMatrixMultiply(scalingMatrix, rotationMatrix);
	matrix = DirectX::XMMatrixMultiply(rotationMatrix, translatingMatrix);
}

void CanvasTransform::calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix) {
	DirectX::XMMATRIX translatingMatrix = DirectX::XMMatrixTranslation(posX, posY, depth);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation));
	DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixScaling(scaleX, scaleY, 1.0f);

	rotationMatrix = DirectX::XMMatrixMultiply(scalingMatrix, rotationMatrix);
	matrix = DirectX::XMMatrixMultiply(rotationMatrix, translatingMatrix);
}

void CanvasTransform::setScaleX(float X) {
	scaleX = X;
	isDirty = true;
}

void CanvasTransform::setScaleY(float Y) {
	scaleY = Y;
	isDirty = true;
}

void CanvasTransform::setRotation(float rot) {
	rotation = rot;
	isDirty = true;
}

void CanvasTransform::setDepth(float dph) {
	depth = dph;
	isDirty = true;
}

void CanvasTransform::setPivotX(float X) {
	pivotX = X;
	isDirty = true;
}

void CanvasTransform::setPivotY(float Y) {
	pivotY = Y;
	isDirty = true;
}

void CanvasTransform::setAnchorX(float X) {
	anchorX = X;
	isDirty = true;
}

void CanvasTransform::setAnchorY(float Y) {
	anchorY = Y;
	isDirty = true;
}

void CanvasTransform::setPositionX(float X) {
	posX = X;
	isDirty = true;
}

void CanvasTransform::setPositionY(float Y) {
	posY = Y;
	isDirty = true;
}

void CanvasTransform::setWidth(float wdth) {
	width = wdth;
	isDirty = true;
}

void CanvasTransform::setHeight(float hght) {
	height = hght;
	isDirty = true;
}

void Transform::markDirty() {
	isDirty = true;
}

void Transform::updateWorldTransformMatrix(const Transform* parentTransform) {
	assert(parentTransform);

	DirectX::XMMATRIX transformMatrix;
	if (parentTransform->getTransformType() == SceneTransformType) {
		calculateLocalTransfomMatrix(transformMatrix, (SceneTransform*)(parentTransform));
	}
	else if (parentTransform->getTransformType() == CanvasTransformType) {
		calculateLocalTransfomMatrix(transformMatrix, (CanvasTransform*)(parentTransform));
	}

	DirectX::XMMATRIX parent = DirectX::XMLoadFloat4x4(parentTransform->worldTransformation);

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
