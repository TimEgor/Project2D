#include "CanvasTransform.h"

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

void CanvasTransform::calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const Transform* parentTransform) {
	const CanvasTransform* parentCanvasTransform = (CanvasTransform*)(parentTransform);

	float currentPosX = (parentCanvasTransform->width * parentCanvasTransform->scaleX) * (parentCanvasTransform->anchorX - 0.5f);
	float currentPosY = (parentCanvasTransform->height * parentCanvasTransform->scaleY) * (parentCanvasTransform->anchorY - 0.5f);
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