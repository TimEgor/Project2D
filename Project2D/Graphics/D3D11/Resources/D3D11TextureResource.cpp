#include "D3D11TextureResource.h"

#include <Graphics/D3D11/D3D11.h>
#include <Graphics/D3D11/WICTextureLoader11.h>

D3D11TextureResource::D3D11TextureResource(void* data, size_t dataSize) {
	ID3D11Device* device = D3D11::get().getDevice();

	DirectX::CreateWICTextureFromMemory(device, (const uint8_t*)(data), dataSize, (ID3D11Resource**)(&texture), &shaderResourceView);
}

D3D11TextureResource::~D3D11TextureResource() {
	D3D11ObjectRelease(texture);
	D3D11ObjectRelease(shaderResourceView);
}
