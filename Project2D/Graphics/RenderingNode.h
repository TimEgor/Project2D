#pragma once

#include <Graphics/Transform.h>
#include <Graphics/Verteces.h>
#include <ResourceManager/ResourceReference.h>

struct RenderingNode {
	TransformMatrix* transform = nullptr;
	ResourceReference materialResource = nullptr;

	RenderingNode() = default;
	RenderingNode(TransformMatrix* transform, ResourceReference material);

	virtual ~RenderingNode() = 0;
};

struct SpriteRenderingNode : public RenderingNode {
	ResourceReference spriteResource = 0;

	SpriteRenderingNode() = default;
	SpriteRenderingNode(TransformMatrix* transform, ResourceReference material, ResourceReference sprite);

	static bool sortPredicate(const RenderingNode* node1, const RenderingNode* node2);
};

//struct TextRenderingOrderNode final : public SpriteRenderingOrderNode {
//	SpriteVertex* verteces = nullptr;
//	uint16_t* indeces = nullptr;
//	uint16_t vertecesCount = 0;
//	uint16_t indecesCount = 0;
//
//	static bool sortPredicate(const RenderingOrderNode* node1, const RenderingOrderNode* node2);
//};