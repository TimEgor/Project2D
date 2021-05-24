#pragma once

#include <EntityManager/EntityComponent.h>
#include <ResourceManager/ResourceReference.h>

class CanvasSpriteRendererEntityComponent final : public EntityComponent {
private:
	ResourceReference sprite;
	ResourceReference material;

public:
	CanvasSpriteRendererEntityComponent();
	CanvasSpriteRendererEntityComponent(ResourceReference spriteResource, ResourceReference materialResource = nullptr);

	ResourceReference getSpriteResource() const { return sprite; }
	void setSpriteResource(ResourceReference newSpriteResource) { sprite = newSpriteResource; }
	ResourceReference getMaterialResource() const { return material; }
	void setMaterialResource(ResourceReference newMaterialResource);

	virtual EntityComponentType getEntityComponentType() const override { return CanvasSpriteRendererEntityComponentType; }
	static EntityComponentType getType() { return CanvasSpriteRendererEntityComponentType; }
};