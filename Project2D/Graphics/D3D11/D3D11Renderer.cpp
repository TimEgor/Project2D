#include "D3D11Renderer.h"

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

    return true;
}

void D3D11Renderer::release() {
    D3D11ObjectRelease(rtv);
}

void D3D11Renderer::testDraw() {
    D3D11& d3d11 = D3D11::get();

    ID3D11Device* device = d3d11.getDevice();
    ID3D11DeviceContext* deviceContext = d3d11.getDeviceContext();
    IDXGISwapChain* swapChain = d3d11.getSwapChain();

    float clearVal[4] = { 0.0f, 0.0f, 0.5f, 1.0f };
    deviceContext->ClearRenderTargetView(rtv, clearVal);

    swapChain->Present(0, 0);
}
