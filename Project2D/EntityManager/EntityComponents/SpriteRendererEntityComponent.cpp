#include "SpriteRendererEntityComponent.h"

#include <ResourceManager/ResourceManager.h>

#include <cassert>

//
//  CustomProperties
//

CustomShaderProperties::CustomShaderProperties() : counter(0), data(nullptr) {}

CustomShaderProperties::~CustomShaderProperties() {
    assert(counter == 0);
    delete[] data;
}

CustomShaderPropertiesReference::CustomShaderPropertiesReference(CustomShaderProperties* properties) : properties(properties) {
    if (properties) {
        ++properties->counter;
    }
}

CustomShaderPropertiesReference::CustomShaderPropertiesReference(const CustomShaderPropertiesReference& reference) : properties(reference.properties) {
    if (properties) {
        ++properties->counter;
    }
}

CustomShaderPropertiesReference::~CustomShaderPropertiesReference() {
    releasePropertiesData();
}

void* CustomShaderPropertiesReference::initNewPropertiesData() {
    releasePropertiesData();
    properties = new CustomShaderProperties();
    ++properties->counter;

    return properties->data;
}

void CustomShaderPropertiesReference::releasePropertiesData() {
    if (properties) {
        if (--properties->counter == 0) {
            delete properties;
        }
    }
}

//
//  SpriteRenderer
//

SpriteRendererEntityComponent::SpriteRendererEntityComponent() : sprite(nullptr) {
    setMaterialResource(nullptr);
}

SpriteRendererEntityComponent::SpriteRendererEntityComponent(ResourceReference spriteResource, ResourceReference materialResource) : sprite(spriteResource) {
    setMaterialResource(materialResource);
}

void SpriteRendererEntityComponent::setMaterialResource(ResourceReference newMaterialResource) {
    if (newMaterialResource.isNull()) {
        newMaterialResource = ResourceManager::get().getResourceFromArchive("Generic/DefaultMaterial.material");
    }

    material = newMaterialResource;
}
