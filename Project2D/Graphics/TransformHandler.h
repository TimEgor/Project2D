#pragma once

#include <Graphics/Transform.h>

class TransformHandler final {
private:
	Transform& transform;
	size_t allocatorIndex;
	TransformID id;

public:
	TransformHandler(TransformID id, Transform& transform, size_t allocatorIndex) : transform(transform), allocatorIndex(allocatorIndex), id(id) {}
	TransformHandler(const TransformHandler&) = delete;
	TransformHandler(TransformHandler&&) = delete;

	Transform& getTransform() { return transform; }

	TransformID getID() { return id; }
	size_t getTransformAllocatorIndex() { return allocatorIndex; }
};