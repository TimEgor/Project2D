#include "D3D11Textures.h"

void D3D11Texture1D::release() {
	D3D11ObjectRelease(texture);
}

void D3D11Texture2D::release() {
	D3D11ObjectRelease(texture);
}

void D3D11Texture3D::release() {
	D3D11ObjectRelease(texture);
}
