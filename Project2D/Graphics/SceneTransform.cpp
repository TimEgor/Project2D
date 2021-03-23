#include "SceneTransform.h"

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

void SceneTransform::calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix) {
	DirectX::XMMATRIX translatingMatrix = DirectX::XMMatrixTranslation(posX, posY, depth);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation));
	DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixScaling(scaleX, scaleY, 1.0f);

	rotationMatrix = DirectX::XMMatrixMultiply(scalingMatrix, rotationMatrix);
	matrix = DirectX::XMMatrixMultiply(rotationMatrix, translatingMatrix);
}
