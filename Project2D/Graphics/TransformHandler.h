#pragma once

#include <Graphics/Transform.h>
#include <MemoryManager/Allocators/PoolAllocatorVector.h>

class TransformHandler final {
private:
	Transform& transform;
	AllocatorID allocatorID;
	TransformID id;

public:
	TransformHandler(TransformID id, Transform& transform, AllocatorID allocatorID) : transform(transform), allocatorID(allocatorID), id(id) {}
	TransformHandler(const TransformHandler&) = delete;
	TransformHandler(TransformHandler&&) = delete;

	Transform& getTransform() { return transform; }

	TransformID getID() { return id; }
	size_t getTransformAllocatorID() { return allocatorID; }
};