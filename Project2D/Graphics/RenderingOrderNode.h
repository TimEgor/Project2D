#pragma once

#include <Graphics/Transform.h>
#include <Graphics/Verteces.h>
#include <ResourceManager/ResourceReference.h>

struct RenderingOrderNode {
	TransformMatrix* transform = nullptr;
	ResourceReference materialResource = nullptr;

	RenderingOrderNode() = default;
	RenderingOrderNode(TransformMatrix * transform, ResourceReference material);

	virtual ~RenderingOrderNode() = 0;
};

struct SpriteRenderingOrderNode : public RenderingOrderNode {
	ResourceReference spriteResource = 0;

	SpriteRenderingOrderNode() = default;
	SpriteRenderingOrderNode(TransformMatrix* transform, ResourceReference material, ResourceReference sprite);

	static bool sortPredicate(const RenderingOrderNode* node1, const RenderingOrderNode* node2);
};

struct TextRenderingOrderNode final : public SpriteRenderingOrderNode {
	SpriteVertex* verteces = nullptr;
	uint16_t* indeces = nullptr;
	uint16_t vertecesCount = 0;
	uint16_t indecesCount = 0;

	static bool sortPredicate(const RenderingOrderNode* node1, const RenderingOrderNode* node2);
};