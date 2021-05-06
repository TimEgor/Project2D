#include "D3D11SpriteBatch.h"

#include <Graphics/D3D11/D3D11Verteces.h>
#include <Graphics/D3D11/D3D11Sprite.h>
#include <Graphics/RenderingOrder.h>

bool D3D11SpriteBatch::init(size_t count) {
	ID3D11Device* device = D3D11::get().getDevice();

    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    //Verteces buffer

    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.ByteWidth = sizeof(D3D11SpriteVertex) * 4 * count;

    device->CreateBuffer(&bufferDesc, nullptr, &vertecesBuffer);


    //Indeces buffer

    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.ByteWidth = sizeof(uint16_t) * 6 * count;

    device->CreateBuffer(&bufferDesc, nullptr, &indecesBuffer);

	return true;
}

void D3D11SpriteBatch::release() {
    D3D11ObjectRelease(vertecesBuffer);
    D3D11ObjectRelease(indecesBuffer);
}

void D3D11SpriteBatch::buildData(RenderingOrderNode* node, size_t count) {
    ID3D11DeviceContext* context = D3D11::get().getDeviceContext();

    D3D11_MAPPED_SUBRESOURCE vertecesMappedSubresource{};
    D3D11_MAPPED_SUBRESOURCE indecesMappedSubresource{};
    context->Map(vertecesBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertecesMappedSubresource);
    context->Map(indecesBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &indecesMappedSubresource);

    D3D11SpriteVertex* vertexPtr = (D3D11SpriteVertex*)(vertecesMappedSubresource.pData);
    uint16_t* indexPtr = (uint16_t*)(indecesMappedSubresource.pData);

    const D3D11SpriteVertex* spriteVerteces = D3D11Sprite::verteces;
    const uint16_t* spriteIndeces = D3D11Sprite::indeces;

    size_t vertNum = 0;
    size_t indexNum = 0;

    for (size_t i = 0; i < count; ++i) {
        RenderingOrderNode* currentNode = node + i;

        for (size_t vertexIndex = 0; vertexIndex < 4; ++vertexIndex) {
            const D3D11SpriteVertex& spriteVertex = spriteVerteces[vertexIndex];

            DirectX::XMFLOAT4 spritePosition{ spriteVertex.posX, spriteVertex.posY, 0.0f, 1.0f };
            DirectX::XMVECTOR position = DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&spritePosition), DirectX::XMLoadFloat4x4(currentNode->transform));
            DirectX::XMStoreFloat4(&spritePosition, position);

            vertexPtr->posX = spritePosition.x;
            vertexPtr->posY = spritePosition.y;
            vertexPtr->texU = spriteVertex.texU;
            vertexPtr->texV = spriteVertex.texV;

            ++vertexPtr;
        }

        indexPtr[0] = spriteIndeces[0] + vertNum;
        indexPtr[1] = spriteIndeces[1] + vertNum;
        indexPtr[2] = spriteIndeces[2] + vertNum;
        indexPtr[3] = spriteIndeces[3] + vertNum;
        indexPtr[4] = spriteIndeces[4] + vertNum;
        indexPtr[5] = spriteIndeces[5] + vertNum;
        indexPtr += 6;

        vertNum += 4;
        indexNum += 6;
    }

    context->Unmap(vertecesBuffer, 0);
    context->Unmap(indecesBuffer, 0);
}
