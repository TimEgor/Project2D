#pragma once

#include <cstdint>

typedef uint32_t TransformID;

class Transform final {
private:
	float posX, posY;
	float scaleX, scaleY;
	float rotation;
	float depth;

public:
	Transform() : posX(0.0f), posY(0.0f),
		scaleX(1.0f), scaleY(1.0f),
		rotation(0.0f), depth(0.0f) {}

	float getPositionX() const { return posX; }
	float getPositionY() const { return posY; }
	void setPositionX(float X) { posX = X; }
	void setPositionY(float Y) { posX = Y; }

	float getScaleX() const { return scaleX; }
	float getScaleY() const { return scaleY; }
	void setScaleX(float X) { scaleX = X; }
	void setScaleY(float Y) { scaleX = Y; }

	float getRotation() const { return rotation; }
	void setRotation(float rot) { rotation = rot; }

	float getDepth() const { return depth; }
	void setDepth(float dph) { depth = dph; }
};