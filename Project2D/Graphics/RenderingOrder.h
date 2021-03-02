#pragma once

#include <ResourceManager/ResourceReference.h>
#include <EntityManager/EntityManagerTypes.h>

#include <MemoryManager/Heap.h>

struct RenderingOrderNode final {
	ResourceReference materialResource;
	ResourceReference spriteResource;
	EntityID entityID;
};

class RenderingOrder final {
private:
	RenderingOrderNode* nodes;

	Heap* heap;

	size_t allocatedNodesSize;
	size_t currentNodeSize;

public:
	RenderingOrder() = default;
	RenderingOrder(const RenderingOrder&) = delete;
	~RenderingOrder() { release(); }

	RenderingOrder& operator=(const RenderingOrder&) = delete;
	const RenderingOrderNode& operator[](size_t index);

	bool init(Heap* heap);
	void release();

	void clear();

	void preReSize(size_t size);
	void pushNode(const RenderingOrderNode& node);
	void pushNode(RenderingOrderNode&& node);

	size_t size() const { return currentNodeSize; }
	size_t capacity() const { return allocatedNodesSize; }
};