#include "Sprite.h"

#include <Graphics/GraphicDeviceManager.h>

Sprite& Sprite::get() {
	static Sprite uniqueSprite;
	return uniqueSprite;
}

bool Sprite::init() {
	GraphicDevice* device = GraphicDeviceManager::get().getCurrentDevice();

	if (!device) {
		return false;
	}

	const float verteces[] = {
		-0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 1.0f, 0.0f
	};

	const uint16_t indeces[] = {
		0, 1, 2,
		0, 2, 3
	};

	SubresourceData data{};

	GPUBufferDesc bufferDesc{};
	bufferDesc.usage = USAGE_IMMUTABLE;
	bufferDesc.cpuAccess = CPU_ACCESS_NULL;
	bufferDesc.structureByteStride = 0;

	data.mem = verteces;

	bufferDesc.bind = BIND_VERTEX_BUFFER;
	bufferDesc.byteSize = sizeof(verteces);

	vertecesBuffer = device->createGPUBuffer(bufferDesc, &data);

	data.mem = indeces;

	bufferDesc.bind = BIND_INDEX_BUFFER;
	bufferDesc.byteSize = sizeof(indeces);

	indecesBuffer = device->createGPUBuffer(bufferDesc, &data);

	return true;
}

void Sprite::release() {
	vertecesBuffer = nullptr;
	indecesBuffer = nullptr;
	inputLayout = nullptr;
}

GPUBufferReference Sprite::getVertecesBuffer() {
	return vertecesBuffer;
}

GPUBufferReference Sprite::getIndecesBuffer() {
	return indecesBuffer;
}
