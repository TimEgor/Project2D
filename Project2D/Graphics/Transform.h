#pragma once

#include <DirectXMath.h>

#include <cstdint>

typedef DirectX::XMFLOAT4X4 TransformMatrix;
typedef uint32_t TransformID;

class Transform {
protected:
	TransformMatrix* worldTransformation;
	bool isDirty;

	virtual void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix) = 0;

public:
	Transform(TransformMatrix* worldTransformationMatrix) :
		worldTransformation(worldTransformationMatrix),
		isDirty(false) {}
	virtual ~Transform() {}

	bool isMatrixDirty() const { return isDirty; }
	void markDirty() { isDirty = true; }

	TransformMatrix* getWorldTransformMatrix() { return worldTransformation; }
	void updateWorldTransformMatrix(const TransformMatrix* parentTransform);
	void updateWorldTransformMatrix();
};