#include "Transform.h"

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
