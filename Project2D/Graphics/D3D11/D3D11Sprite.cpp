#include "D3D11Sprite.h"

#include <Graphics/D3D11/D3D11Verteces.h>

D3D11Sprite::D3D11Sprite() :
    vertecesBuffer(nullptr), vertecesNum(0),
    indecesBuffer(nullptr), indecesNum(0) {}

D3D11Sprite& D3D11Sprite::get() {
    static D3D11Sprite uniqueD3D11Sprite;
    return uniqueD3D11Sprite;
}

bool D3D11Sprite::init() {
    D3D11SpriteVertex verteces[4] = {
        { -0.5f, 0.5f, 0.0f, 0.0f },
        { -0.5f, -0.5f, 0.0f, 1.0f },
        { 0.5f, -0.5f, 1.0f, 1.0f },
        { 0.5f, 0.5f, 1.0f, 0.0f }
    };

    uint32_t indeces[6] = {
        0, 1, 2,
        0, 2, 3
    };

    D3D11& d3d11 = D3D11::get();
    ID3D11Device* device = d3d11.getDevice();

    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA subresourcesData{};

    //Verteces buffer

    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.ByteWidth = sizeof(D3D11SpriteVertex) * 4;

    subresourcesData.pSysMem = verteces;

    device->CreateBuffer(&bufferDesc, &subresourcesData, &vertecesBuffer);

    //Indeces buffer

    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.ByteWidth = sizeof(size_t) * 6;

    subresourcesData.pSysMem = indeces;

    device->CreateBuffer(&bufferDesc, &subresourcesData, &indecesBuffer);

    return true;
}

void D3D11Sprite::release() {
    D3D11ObjectRelease(vertecesBuffer);
    D3D11ObjectRelease(indecesBuffer);
}
