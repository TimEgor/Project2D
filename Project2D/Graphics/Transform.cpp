#include "Transform.h"

void Transform::updateWorldTransformMatrix(const Transform* parentTransform) {
	assert(parentTransform);

	DirectX::XMMATRIX transformMatrix;
	calculateLocalTransfomMatrix(transformMatrix, parentTransform);

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
