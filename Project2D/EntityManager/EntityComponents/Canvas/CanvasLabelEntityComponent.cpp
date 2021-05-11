#include "CanvasLabelEntityComponent.h"

#include <MemoryManager/MemoryManager.h>
#include <ResourceManager/ResourceManager.h>
#include <Graphics/FontManager.h>
#include <Graphics/TextureResource.h>

#include <cmath>
#include <cassert>

CanvasLabelEntityComponent::CanvasLabelEntityComponent() : CanvasLabelEntityComponent("") {}

CanvasLabelEntityComponent::CanvasLabelEntityComponent(Heap* heap) : CanvasLabelEntityComponent("", heap) {}

CanvasLabelEntityComponent::CanvasLabelEntityComponent(const char* text) : CanvasLabelEntityComponent(text, MemoryManager::get().getDefaultHeap()) {}

CanvasLabelEntityComponent::CanvasLabelEntityComponent(const char* text, Heap* heap)
    : text(CustomHeapAllocator<LabelTextChar>(heap)), heap(heap) {
    setText(text);
    setMaterialResource(nullptr);
}

CanvasLabelEntityComponent::~CanvasLabelEntityComponent() {
	release();
}

void CanvasLabelEntityComponent::setText(const char* newText) {
    text = newText;
    isDirty = true;
}

void CanvasLabelEntityComponent::setMaterialResource(ResourceReference newMaterialResource) {
    if (newMaterialResource.isNull()) {
        newMaterialResource = ResourceManager::get().getResourceFromArchive("Generic/DefaultFontMaterial.material");
    }

    materialResource = newMaterialResource;
}

void CanvasLabelEntityComponent::update() {
    if (isDirty) {
        updateBuffers();

        isDirty = false;
    }
}

void CanvasLabelEntityComponent::release() {
	if (verteces) {
		heap->deallocate(verteces);
		verteces = nullptr;
	}

	if (indeces) {
		heap->deallocate(indeces);
		indeces = nullptr;
	}
}

void CanvasLabelEntityComponent::updateBuffers() {
	if (text.empty()) {
		return;
	}

	size_t textSize = text.size();
	if (textSize > allocatedCharSize) {
		release();

		verteces = (SpriteVertex*)(heap->allocate(textSize * 4 * sizeof(SpriteVertex)));
		indeces = (uint16_t*)(heap->allocate(textSize * 6 * sizeof(uint16_t)));
		allocatedCharSize = textSize;
	}

	const FontInfo* fontInfo = FontManager::get().getFontInfo(fontID);
	if (fontInfo) {
		size_t vertexPos = 0;
		size_t indexPos = 0;

		float posX = 0.0f;
		float maxPosY = 0.0f;

		const TextureResource& bitmap = fontInfo->bitmapTexture.getResource<TextureResource>();

		size_t bitmapWight = bitmap.getWidth();
		size_t bitmapHeight = bitmap.getHeight();

		for (size_t i = 0; i < textSize; ++i) {
			LabelTextChar currentChar = text[i];

			auto charFontInfoIter = fontInfo->chars.find(currentChar);
			if (charFontInfoIter != fontInfo->chars.end()) {
				const FontCharInfo& charInfo = charFontInfoIter->second;

				int16_t kerning = 0;
				if (i > 0) {
					CharID lastChar = text[i - 1];
					kerning = fontInfo->getKerning(currentChar, lastChar);
				}

				posX += (charInfo.xoffset + kerning) / (float)(fontInfo->lineHeight);

				float posYTop = -charInfo.yoffset / (float)(fontInfo->lineHeight);
				float posYDown = -(charInfo.yoffset + charInfo.height) / (float)(fontInfo->lineHeight);

				float bitmapU = charInfo.x / (float)(bitmapWight);
				float bitmapV = charInfo.y / (float)(bitmapHeight);
				float bitmapUF = (charInfo.x + charInfo.width) / (float)(bitmapWight);
				float bitmapVF = (charInfo.y + charInfo.height) / (float)(bitmapHeight);

				if (maxPosY > posYDown) {
					maxPosY = posYDown;
				}

				//Left top
				verteces[vertexPos].posX = posX;
				verteces[vertexPos].posY = posYTop;
				verteces[vertexPos].texU = bitmapU;
				verteces[vertexPos].texV = bitmapV;

				//Left bottom
				verteces[vertexPos + 1].posX = posX;
				verteces[vertexPos + 1].posY = posYDown;
				verteces[vertexPos + 1].texU = bitmapU;
				verteces[vertexPos + 1].texV = bitmapVF;

				posX += charInfo.width / (float)(fontInfo->lineHeight);

				//Right bottom
				verteces[vertexPos + 2].posX = posX;
				verteces[vertexPos + 2].posY = posYDown;
				verteces[vertexPos + 2].texU = bitmapUF;
				verteces[vertexPos + 2].texV = bitmapVF;

				//Right top
				verteces[vertexPos + 3].posX = posX;
				verteces[vertexPos + 3].posY = posYTop;
				verteces[vertexPos + 3].texU = bitmapUF;
				verteces[vertexPos + 3].texV = bitmapV;

				indeces[indexPos] = vertexPos;
				indeces[indexPos + 1] = vertexPos + 1;
				indeces[indexPos + 2] = vertexPos + 2;
				indeces[indexPos + 3] = vertexPos;
				indeces[indexPos + 4] = vertexPos + 2;
				indeces[indexPos + 5] = vertexPos + 3;

				vertexPos += 4;
				indexPos += 6;
			}
		}

		rect.x = 0.0f;
		rect.y = 0.0f;
		rect.width = posX;
		rect.height = maxPosY;

		vertecesCount = vertexPos;
		indecesCount = indexPos;
	}
	else {
		assert(false && "FontInfo is null.");
	}
}