#include "D3D11TestRenderer.h"

#include <Graphics/D3D11/D3D11Sprite.h>
#include <Graphics/D3D11/Resources/D3D11MaterialResource.h>
#include <Graphics/D3D11/Resources/D3D11ShaderResource.h>
#include <Graphics/D3D11/D3D11Verteces.h>
#include <ResourceManager/ResourceManager.h>

D3D11TestRenderer::D3D11TestRenderer() : rtv(nullptr) {}

D3D11TestRenderer& D3D11TestRenderer::get() {
    static D3D11TestRenderer uniqueD3D11Renderer;
    return uniqueD3D11Renderer;
}

bool D3D11TestRenderer::init() {
    D3D11& d3d11 = D3D11::get();

    ID3D11Device* device = d3d11.getDevice();
    IDXGISwapChain* swapChain = d3d11.getSwapChain();

    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(backBuffer), (void**)(&backBuffer));
    device->CreateRenderTargetView(backBuffer, nullptr, &rtv);

    D3D11_RASTERIZER_DESC rastStateDesc{};
    rastStateDesc.CullMode = D3D11_CULL_BACK;
    rastStateDesc.FillMode = D3D11_FILL_SOLID;
    rastStateDesc.FrontCounterClockwise = false;

    device->CreateRasterizerState(&rastStateDesc, &rastState);

    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = 800.0f;
    viewport.Height = 800.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    D3D11Sprite::get().init();

    pixelShader = ResourceManager::get().getResourceFromArchive("Generic/TestSpriteDefaultPixelShader.pshader");
    vertexShader = ResourceManager::get().getResourceFromArchive("Generic/TetsSpriteDefaultVertexShader.vshader");

    return true;
}

void D3D11TestRenderer::release() {
    D3D11ObjectRelease(rtv);
    D3D11ObjectRelease(rastState);
}

void D3D11TestRenderer::render() {
    D3D11& d3d11 = D3D11::get();

    ID3D11Device* device = d3d11.getDevice();
    ID3D11DeviceContext* deviceContext = d3d11.getDeviceContext();
    IDXGISwapChain* swapChain = d3d11.getSwapChain();

    float clearVal[4] = { 0.0f, 0.0f, 0.5f, 1.0f };
    deviceContext->ClearRenderTargetView(rtv, clearVal);

    D3D11Sprite& d3d11Sprite = D3D11Sprite::get();

    ID3D11Buffer* vertecesBuffer = d3d11Sprite.getVertecesBuffer();
    UINT strides = sizeof(D3D11SpriteVertex);
    UINT offsets = 0;

    deviceContext->IASetVertexBuffers(0, 1, &vertecesBuffer, &strides, &offsets);
    deviceContext->IASetIndexBuffer(d3d11Sprite.getIndecesBuffer(), DXGI_FORMAT_R32_UINT, 0);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    D3D11VertexShaderResource& vertexShaderResource = vertexShader.getResource<D3D11VertexShaderResource>();
    D3D11PixelShaderResource& pixelShaderResource = pixelShader.getResource<D3D11PixelShaderResource>();

    deviceContext->IASetInputLayout(vertexShaderResource.getInputLayout());
    deviceContext->VSSetShader(vertexShaderResource.getVertexShader(), nullptr, 0);
    deviceContext->PSSetShader(pixelShaderResource.getPixelShader(), nullptr, 0);

    deviceContext->RSSetState(rastState);
    deviceContext->RSSetViewports(1, &viewport);

    deviceContext->OMSetRenderTargets(1, &rtv, nullptr);

    deviceContext->DrawIndexed(6, 0, 0);

    swapChain->Present(0, 0);
}
