#include "CanvasSpriteRendererEntityComponent.h"

#include <ResourceManager/ResourceManager.h>

CanvasSpriteRendererEntityComponent::CanvasSpriteRendererEntityComponent() : sprite(nullptr) {
	setMaterialResource(nullptr);
}

CanvasSpriteRendererEntityComponent::CanvasSpriteRendererEntityComponent(ResourceReference spriteResource, ResourceReference materialResource)
	: sprite(spriteResource) {
	setMaterialResource(materialResource);
}

void CanvasSpriteRendererEntityComponent::setMaterialResource(ResourceReference newMaterialResource) {
	if (newMaterialResource.isNull()) {
		newMaterialResource = ResourceManager::get().getResourceFromArchive("Generic/DefaultMaterial.material");
	}

	material = newMaterialResource;
}
