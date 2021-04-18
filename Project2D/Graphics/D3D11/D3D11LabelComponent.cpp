#include "D3D11LabelComponent.h"

#include <Graphics/D3D11/D3D11Verteces.h>
#include <Graphics/D3D11/Resources/D3D11TextureResource.h>
#include <Graphics/FontManager.h>
#include <ResourceManager/ResourceManager.h>

void D3D11LabelComponent::updateBuffers() {
	if (text.empty()) {
		release();
		return;
	}

	size_t textSize = text.size();
	if (textSize > allocatedCharSize) {
		release();
	}

	D3D11& d3d11 = D3D11::get();
	ID3D11Device* d3d11Device = d3d11.getDevice();
	ID3D11DeviceContext* deviceContext = d3d11.getDeviceContext();

	const FontInfo* fontInfo = FontManager::get().getFontInfo(fontID);
	if (fontInfo) {
		D3D11SpriteVertex* verteces = new D3D11SpriteVertex[textSize * 4];
		size_t* indeces = new size_t[textSize * 6];

		size_t vertexPos = 0;
		size_t indexPos = 0;

		float posX = 0.0f;
		float maxPosY = 0.0f;

		D3D11TextureResource& bitmap = fontInfo->bitmapTexture.getResource<D3D11TextureResource>();

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

		indexCount = indexPos;

		if (allocatedCharSize == 0) {
			D3D11ObjectRelease(vertexBuffer);
			D3D11ObjectRelease(indexBuffer);

			D3D11_BUFFER_DESC bufferDesc{};
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			
			D3D11_SUBRESOURCE_DATA subresourcesData{};

			bufferDesc.ByteWidth = sizeof(D3D11SpriteVertex) * textSize * 4;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			subresourcesData.pSysMem = verteces;
			d3d11Device->CreateBuffer(&bufferDesc, &subresourcesData, &vertexBuffer);

			bufferDesc.ByteWidth = sizeof(size_t) * textSize * 6;
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			subresourcesData.pSysMem = indeces;
			d3d11Device->CreateBuffer(&bufferDesc, &subresourcesData, &indexBuffer);

			allocatedCharSize = textSize;
		}
		else {
			D3D11_MAPPED_SUBRESOURCE mappedResource{};

			deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			D3D11SpriteVertex* verticesPtr = (D3D11SpriteVertex*)(mappedResource.pData);
			memcpy(verticesPtr, verteces, sizeof(D3D11SpriteVertex) * vertexPos);

			deviceContext->Unmap(vertexBuffer, 0);


			deviceContext->Map(indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			size_t* indecesPtr = (size_t*)(mappedResource.pData);
			memcpy(indecesPtr, indecesPtr, sizeof(size_t) * indexPos);

			deviceContext->Unmap(indexBuffer, 0);
		}

		delete[] verteces;
		delete[] indeces;
	}
	else {
		assert(false && "FontInfo is null.");
	}
}

void D3D11LabelComponent::release() {
	D3D11ObjectRelease(vertexBuffer);
	D3D11ObjectRelease(indexBuffer);

	allocatedCharSize = 0;
}
