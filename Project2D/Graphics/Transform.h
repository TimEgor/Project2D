#pragma once

#include <Graphics/SceneTypes.h>

#include <DirectXMath.h>
#include <cstdint>

typedef DirectX::XMFLOAT4X4 TransformMatrix;


class Transform {
protected:
	TransformMatrix* worldTransformation;

	bool isDirty;

	virtual void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const SceneTransform* parentTransform) = 0;
	virtual void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const CanvasTransform* parentTransform) = 0;
	virtual void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix) = 0;

public:
	Transform(TransformMatrix* worldTransformationMatrix) :
		worldTransformation(worldTransformationMatrix),
		isDirty(false) {}

	~Transform() {}

	bool isMatrixDirty() const { return isDirty; }
	void markDirty();

	TransformMatrix* getWorldTransformMatrix() { return worldTransformation; }
	virtual void updateWorldTransformMatrix(const Transform* parentTransform);
	virtual void updateWorldTransformMatrix();

	virtual TransformType getTransformType() const = 0;
};

class SceneTransform final : public Transform {
private:
	float posX, posY;
	float scaleX, scaleY;
	float rotation;
	float depth;

	virtual void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const SceneTransform* parentTransform) override;
	virtual void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const CanvasTransform* parentTransform) override;
	virtual void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix) override;

public:
	SceneTransform(TransformMatrix* worldTransformationMatrix) :
		Transform(worldTransformationMatrix),
		posX(0.0f), posY(0.0f),
		scaleX(1.0f), scaleY(1.0f),
		rotation(0.0f), depth(0) {}


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

	float getDepth() const { return depth; }
	void setDepth(float dph);

	virtual TransformType getTransformType() const override { return SceneTransformType; }
};

class CanvasTransform final : public Transform {
private:
	float scaleX, scaleY;
	float rotation;
	float depth;
	float pivotX, pivotY;
	float anchorX, anchorY;
	float posX, posY;
	float width, height;

	bool isPreparing;

	virtual void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const SceneTransform* parentTransform) override;
	virtual void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix, const CanvasTransform* parentTransform) override;
	virtual void calculateLocalTransfomMatrix(DirectX::XMMATRIX& matrix) override;

public:
	CanvasTransform(TransformMatrix* worldTransformationMatrix) : Transform(worldTransformationMatrix),
		scaleX(1.0f), scaleY(1.0f),
		rotation(0.0f), depth(0.0f),
		pivotX(0.5f), pivotY(0.5f),
		anchorX(0.5f), anchorY(0.5f),
		posX(0.0f), posY(0.0f),
		width(100.0f), height(100.0f),
		isPreparing(false) {
	}

	float getScaleX() const { return scaleX; }
	float getScaleY() const { return scaleY; }
	void setScaleX(float X);
	void setScaleY(float Y);

	float getRotation() const { return rotation; }
	void setRotation(float rot);

	float getDepth() const { return depth; }
	void setDepth(float dph);

	float getPivotX() const { return pivotX; }
	float getPivotY() const { return pivotY; }
	void setPivotX(float X);
	void setPivotY(float Y);

	float getAnchorX() const { return anchorX; }
	float getAnchorY() const { return anchorY; }
	void setAnchorX(float X);
	void setAnchorY(float Y);

	float getPositionX() const { return posX; }
	float getPositionY() const { return posY; }
	void setPositionX(float X);
	void setPositionY(float Y);

	float getWidth() const { return width; }
	float getHeight() const { return height; }
	void setWidth(float width);
	void setHeight(float height);

	virtual TransformType getTransformType() const override { return CanvasTransformType; }
};