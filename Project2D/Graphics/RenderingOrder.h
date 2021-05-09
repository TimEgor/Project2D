#pragma once

#include <ResourceManager/ResourceReference.h>
#include <EntityManager/EntityManagerTypes.h>
#include <Graphics/Transform.h>

#include <MemoryManager/Heap.h>

enum RenderingOrderType {
	SceneOrderType,
	CanvasOrderType
};

struct RenderingOrderNode final {
	ResourceReference materialResource;
	ResourceReference spriteResource;
	TransformMatrix* transform;
	EntityID entityID;

	RenderingOrderNode() : materialResource(nullptr), spriteResource(nullptr),
		transform(nullptr), entityID(0) {}
};

class RenderingOrder final {
private:
	RenderingOrderNode* nodes;
	TransformMatrix* transforms;

	Heap* heap;

	size_t allocatedNodesSize;
	size_t currentNodeSize;

	RenderingOrderType type;

public:
	RenderingOrder(RenderingOrderType type);
	RenderingOrder(const RenderingOrder&) = delete;
	~RenderingOrder() { release(); }

	RenderingOrder& operator=(const RenderingOrder&) = delete;
	RenderingOrderNode& operator[](size_t index);
	const RenderingOrderNode& operator[](size_t index) const;

	bool init(Heap* heap);
	void release();

	void clear();

	void preReSize(size_t size);
	void pushNode(EntityID entityID, ResourceReference materialResource, ResourceReference spriteResource, const TransformMatrix* transform);

	void sort();

	size_t size() const { return currentNodeSize; }
	size_t capacity() const { return allocatedNodesSize; }

	RenderingOrderType getType() const { return type; }
};