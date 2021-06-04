#include "CanvasSpriteRendererEntityComponent.h"

#include <ResourceManager/ResourceManager.h>

CanvasSpriteRendererEntityComponent::CanvasSpriteRendererEntityComponent(EntityComponentHandler* handler) : EntityComponent(handler), sprite(nullptr) {
	setMaterialResource(nullptr);
}

CanvasSpriteRendererEntityComponent::CanvasSpriteRendererEntityComponent(EntityComponentHandler* handler, ResourceReference spriteResource, ResourceReference materialResource)
	: EntityComponent(handler), sprite(spriteResource) {
	setMaterialResource(materialResource);
}

void CanvasSpriteRendererEntityComponent::setMaterialResource(ResourceReference newMaterialResource) {
	if (newMaterialResource.isNull()) {
		newMaterialResource = ResourceManager::get().getResourceFromArchive("Generic/DefaultMaterial.material");
	}

	material = newMaterialResource;
}
