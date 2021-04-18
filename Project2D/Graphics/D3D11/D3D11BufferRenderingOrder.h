#pragma once

#include <Graphics/D3D11/D3D11.h>
#include <Graphics/RenderingOrder.h>
#include <ResourceManager/ResourceReference.h>
#include <EntityManager/EntityManagerTypes.h>

struct D3D11BufferRenderingOrderNode final {
	ResourceReference materialResource = nullptr;
	ResourceReference spriteResource = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	size_t indexCount = 0;
	TransformMatrix* transform = nullptr;
	EntityID entityID = 0;
};

class D3D11BufferRenderingOrder final : public RenderingOrder<D3D11BufferRenderingOrderNode> {
public:
	D3D11BufferRenderingOrder() {}

	void pushNode(EntityID entityID, ResourceReference materialResource, ResourceReference spriteResource,
		ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, size_t indexCount, const TransformMatrix* transform);
	virtual void sort() override;
};