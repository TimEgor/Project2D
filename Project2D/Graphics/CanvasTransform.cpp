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

void CanvasTransform::setPositionX(uint16_t X) {
	posX = X;
	isDirty = true;
}

void CanvasTransform::setPositionY(uint16_t Y) {
	posY = Y;
	isDirty = true;
}

void CanvasTransform::calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix) {
	DirectX::XMMATRIX translatingMatrix = DirectX::XMMatrixTranslation(posX, posY, depth);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation));
	DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixScaling(scaleX, scaleY, 1.0f);

	rotationMatrix = DirectX::XMMatrixMultiply(scalingMatrix, rotationMatrix);
	matrix = DirectX::XMMatrixMultiply(rotationMatrix, translatingMatrix);
}