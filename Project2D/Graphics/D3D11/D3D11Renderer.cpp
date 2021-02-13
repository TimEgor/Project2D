#include "D3D11Renderer.h"

#include <Graphics/D3D11/D3D11Sprite.h>
#include <Graphics/D3D11/Resources/D3D11MaterialResource.h>
#include <Graphics/D3D11/Resources/D3D11ShaderResource.h>
#include <Graphics/D3D11/D3D11Verteces.h>
#include <ResourceManager/ResourceManager.h>

D3D11Renderer::D3D11Renderer() : rtv(nullptr) {}

D3D11Renderer& D3D11Renderer::get() {
    static D3D11Renderer uniqueD3D11Renderer;
    return uniqueD3D11Renderer;
}

bool D3D11Renderer::init() {
    D3D11& d3d11 = D3D11::get();

    ID3D11Device* device = d3d11.getDevice();
    IDXGISwapChain* swapChain = d3d11.getSwapChain();

    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(backBuffer), (void**)(&backBuffer));
    device->CreateRenderTargetView(backBuffer, nullptr, &rtv);

    D3D11Sprite::get().init();

    return true;
}

void D3D11Renderer::release() {
    D3D11ObjectRelease(rtv);
}