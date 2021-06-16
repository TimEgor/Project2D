#include "CanvasLabelEntityComponent.h"

#include <ResourceManager/ResourceManager.h>
#include <Graphics/FontManager.h>
#include <Graphics/Resources/TextureResource.h>

#include <cmath>
#include <cassert>

CanvasLabelEntityComponent::CanvasLabelEntityComponent(EntityComponentHandler* handler) : CanvasLabelEntityComponent(handler, "") {}

CanvasLabelEntityComponent::CanvasLabelEntityComponent(EntityComponentHandler* handler, const char* text)
	: EntityComponent(handler) {
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
		delete[] verteces;
		verteces = nullptr;
	}

	if (indeces) {
		delete[] indeces;
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

		verteces = new SpriteVertex[textSize * 4];
		indeces = new uint16_t[textSize * 6];
		allocatedCharSize = textSize;
	}

	const FontInfo* fontInfo = FontManager::get().getFontInfo(fontID);
	if (fontInfo) {
		uint16_t vertexPos = 0;
		uint16_t indexPos = 0;

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

				posX += charInfo.xoffset + kerning;

				float posYTop = (float)(-charInfo.yoffset);
				float posYDown = (float)(-(charInfo.yoffset + charInfo.height));

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

				posX += charInfo.width;

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
				indeces[indexPos + 1] = vertexPos + 1u;
				indeces[indexPos + 2] = vertexPos + 2u;
				indeces[indexPos + 3] = vertexPos;
				indeces[indexPos + 4] = vertexPos + 2u;
				indeces[indexPos + 5] = vertexPos + 3u;

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