#pragma once

#include <Graphics/RenderingOrder.h>
#include <ResourceManager/ResourceReference.h>
#include <EntityManager/EntityManagerTypes.h>

struct SpriteRenderingOrderNode final {
	ResourceReference materialResource = nullptr;
	ResourceReference spriteResource = nullptr;
	TransformMatrix* transform = nullptr;
	EntityID entityID = 0;
};

class SpriteRenderingOrder final : public RenderingOrder<SpriteRenderingOrderNode> {
public:
	SpriteRenderingOrder() {}

	void pushNode(EntityID entityID, ResourceReference materialResource, ResourceReference spriteResource, const TransformMatrix* transform);
	virtual void sort() override;
};