#pragma once

#include <Graphics/Transform.h>

#include <atomic>

class TransformHandler final {
private:
	Transform& transform;
	std::size_t counter;
	size_t allocatorIndex;
	TransformID id;

public:
	TransformHandler(TransformID id, Transform& transform, size_t allocatorIndex) : transform(transform), counter(0), allocatorIndex(allocatorIndex), id(id) {}
	TransformHandler(const TransformHandler&) = delete;
	TransformHandler(TransformHandler&&) = delete;

	Transform& getTransform() { return transform; }

	TransformID getID() { return id; }
	size_t getTransformAllocatorIndex() { return allocatorIndex; }

	size_t getCounter() { return counter; }
	void incrementCounter() { ++counter; }
	void decrementCounter() { --counter; }
};