#include "LabelComponent.h"

#include <MemoryManager/MemoryManager.h>
#include <ResourceManager/ResourceManager.h>

#include <cmath>

LabelComponent::LabelComponent() : LabelComponent("") {}

LabelComponent::LabelComponent(Heap* heap) : LabelComponent("", heap) {}

LabelComponent::LabelComponent(const char* text) : LabelComponent(text, MemoryManager::get().getDefaultHeap()) {}

LabelComponent::LabelComponent(const char* text, Heap* heap)
    : text(CustomHeapAllocator<LabelTextChar>(heap)) {
    setText(text);
    setMaterialResource(nullptr);
}

void LabelComponent::setText(const char* newText) {
    text = newText;
    isDirty = true;
}

void LabelComponent::setMaterialResource(ResourceReference newMaterialResource) {
    if (newMaterialResource.isNull()) {
        newMaterialResource = ResourceManager::get().getResourceFromArchive("Generic/DefaultFontMaterial.material");
    }

    materialResource = newMaterialResource;
}

void LabelComponent::update() {
    if (isDirty) {
        updateBuffers();

        isDirty = false;
    }
}
