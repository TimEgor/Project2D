#pragma once

#include <Graphics/GraphicDevice.h>

class Sprite final {
private:
	GPUBufferReference vertecesBuffer;
	GPUBufferReference indecesBuffer;

	Sprite() = default;

public:
	~Sprite() {}

	static Sprite& get();

	bool init();
	void release();

	GPUBufferReference getVertecesBuffer();
	GPUBufferReference getIndecesBuffer();
};