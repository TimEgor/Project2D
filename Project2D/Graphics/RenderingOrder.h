#pragma once

#include <ResourceManager/ResourceReference.h>
#include <EntityManager/EntityManagerTypes.h>
#include <Graphics/Transform.h>
#include <Graphics/Verteces.h>

#include <vector>

enum RenderingOrderType {
	SceneOrderType,
	CanvasOrderType
};

enum RenderingOrderNodeType {
	SpriteOrderNodeType,
	BatchSpriteOrderNodeType
};

struct RenderingOrderNode {
	ResourceReference materialResource = nullptr;
	ResourceReference spriteResource = nullptr;
	TransformMatrix* transform = nullptr;

	RenderingOrderNode() = default;
	virtual ~RenderingOrderNode() {}

	virtual RenderingOrderNodeType getType() const { return SpriteOrderNodeType; }
};

struct BatchRenderingOrderNode final : public RenderingOrderNode {
	SpriteVertex* verteces = nullptr;
	uint16_t* indeces = nullptr;
	uint16_t vertecesCount = 0;
	uint16_t indecesCount = 0;

	BatchRenderingOrderNode() = default;

	virtual RenderingOrderNodeType getType() const override { return BatchSpriteOrderNodeType; }
};

class RenderingOrder final {
private:
	std::vector<RenderingOrderNode*> nodes;
	std::vector<RenderingOrderNode> spritesNodes;
	std::vector<BatchRenderingOrderNode> batchSpritesNode;
	std::vector<TransformMatrix> transforms;

	RenderingOrderType type;

public:
	RenderingOrder(RenderingOrderType type);
	RenderingOrder(const RenderingOrder&) = delete;
	~RenderingOrder() { release(); }

	RenderingOrder& operator=(const RenderingOrder&) = delete;
	RenderingOrderNode* operator[](size_t index);
	const RenderingOrderNode* operator[](size_t index) const;

	const std::vector<RenderingOrderNode*>& getNodes() const { return nodes; }

	void release();

	void clear();

	void reserve(size_t size);
	void pushNode(ResourceReference materialResource, ResourceReference spriteResource, const TransformMatrix* transform);
	void pushBatchNode(ResourceReference materialResource, ResourceReference spriteResource, const TransformMatrix* transform,
		uint16_t vertecesCount, SpriteVertex* verteces, uint16_t indecesCount, uint16_t* indeces);

	void sort();

	size_t size() const { return nodes.size(); }

	RenderingOrderType getType() const { return type; }
};