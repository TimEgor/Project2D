#include "D3D11TestRenderer.h"

#include <Graphics/D3D11/D3D11Sprite.h>
#include <Graphics/D3D11/Resources/D3D11MaterialResource.h>
#include <Graphics/D3D11/Resources/D3D11ShaderResource.h>
#include <Graphics/D3D11/Resources/D3D11TextureResource.h>
#include <Graphics/D3D11/D3D11Verteces.h>
#include <ResourceManager/ResourceManager.h>

D3D11TestRenderer::D3D11TestRenderer() : rtv(nullptr), rastState(nullptr),
    dsv(nullptr), dsBuffer(nullptr),
    spriteSamplerState(nullptr), perObjectTransformBuffer(nullptr) {}

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

    D3D11_TEXTURE2D_DESC dsBufferDesc{};
    dsBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsBufferDesc.Width = 800;
    dsBufferDesc.Height = 800;
    dsBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    dsBufferDesc.MiscFlags = 0;
    dsBufferDesc.ArraySize = 1;
    dsBufferDesc.CPUAccessFlags = 0;
    dsBufferDesc.MipLevels = 1;
    dsBufferDesc.SampleDesc.Count = 1;
    dsBufferDesc.SampleDesc.Quality = 0;
    dsBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    
    device->CreateTexture2D(&dsBufferDesc, nullptr, &dsBuffer);

    device->CreateDepthStencilView(dsBuffer, nullptr, &dsv);

    D3D11_SAMPLER_DESC spriteSamplerDesc{};
    spriteSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    spriteSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    spriteSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    spriteSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    spriteSamplerDesc.MipLODBias = 0.0f;
    spriteSamplerDesc.MaxAnisotropy = 1;
    spriteSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    spriteSamplerDesc.MinLOD = -FLT_MAX;
    spriteSamplerDesc.MaxLOD = FLT_MAX;

    device->CreateSamplerState(&spriteSamplerDesc, &spriteSamplerState);

    ResourceManager& resourceManager = ResourceManager::get();
    pixelShader = resourceManager.getResourceFromArchive("Generic/TestSpriteDefaultPixelShader.pshader");
    vertexShader = resourceManager.getResourceFromArchive("Generic/TestSpriteDefaultVertexShader.vshader");
    sprite = resourceManager.getResourceFromArchive("testtexture.png");

    D3D11_BUFFER_DESC perObjectTransformMappedBufferDesc;
    perObjectTransformMappedBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    perObjectTransformMappedBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    perObjectTransformMappedBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    perObjectTransformMappedBufferDesc.MiscFlags = NULL;
    perObjectTransformMappedBufferDesc.StructureByteStride = NULL;
    perObjectTransformMappedBufferDesc.ByteWidth = sizeof(PerObjectTransforms);

    device->CreateBuffer(&perObjectTransformMappedBufferDesc, nullptr, &perObjectTransformBuffer);

    return true;
}

void D3D11TestRenderer::release() {
    D3D11ObjectRelease(rtv);
    D3D11ObjectRelease(rastState);

    D3D11ObjectRelease(spriteSamplerState);

    D3D11ObjectRelease(perObjectTransformBuffer);
}

void D3D11TestRenderer::prepareViewTransformMatrix(DirectX::XMMATRIX& viewTransform) {
    DirectX::XMVECTOR position{ 0.0f, 0.0f, -5.0f };
    DirectX::XMVECTOR direction{ 0.0f, 0.0f, 1.0f };
    DirectX::XMVECTOR up{ 0.0f, 1.0f, 0.0f };
    viewTransform = DirectX::XMMatrixLookToLH(position, direction, up);
}

void D3D11TestRenderer::prepareProjTransformMatrix(DirectX::XMMATRIX& projTransform) {
    float ratio = atanf(45.0f / 2.0f) * 2.0f;
    float aspect = 1.0f;
    float z = 1.0f;

    float x = ratio * z;
    float y = ratio * z * aspect;

    projTransform = DirectX::XMMatrixOrthographicOffCenterLH(-x, x, -y, y, 0.0f, 1000.0f);
}

void D3D11TestRenderer::render() {
    D3D11& d3d11 = D3D11::get();

    ID3D11Device* device = d3d11.getDevice();
    ID3D11DeviceContext* deviceContext = d3d11.getDeviceContext();
    IDXGISwapChain* swapChain = d3d11.getSwapChain();

    float clearVal[4] = { 0.0f, 0.0f, 0.5f, 1.0f };
    deviceContext->ClearRenderTargetView(rtv, clearVal);
    deviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    D3D11Sprite& d3d11Sprite = D3D11Sprite::get();

    DirectX::XMMATRIX viewTransform;
    prepareViewTransformMatrix(viewTransform);
    DirectX::XMMATRIX projTransform;
    prepareProjTransformMatrix(projTransform);

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

    deviceContext->OMSetRenderTargets(1, &rtv, dsv);

    deviceContext->VSSetConstantBuffers(1, 1, &perObjectTransformBuffer);

    ID3D11ShaderResourceView* spriteSRV = sprite.getResource<D3D11TextureResource>().getShaderResoruceView();
    deviceContext->PSSetShaderResources(0, 1, &spriteSRV);
    deviceContext->PSSetSamplers(0, 1, &spriteSamplerState);

    //

    DirectX::XMMATRIX worldTransform = DirectX::XMMatrixTranslation(0.1f, 0.1f, 0.0f);
    DirectX::XMMATRIX scaleTransform = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
    worldTransform = DirectX::XMMatrixMultiply(scaleTransform, worldTransform);

    PerObjectTransforms* mappedTransforms;
    D3D11_MAPPED_SUBRESOURCE mappedSubresource{};
    deviceContext->Map(perObjectTransformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

    mappedTransforms = (PerObjectTransforms*)(mappedSubresource.pData);
    mappedTransforms->worldTransformMatrix = DirectX::XMMatrixTranspose(worldTransform);
    mappedTransforms->viewTransformMatrix = DirectX::XMMatrixTranspose(viewTransform);
    mappedTransforms->projTransformMatrix= DirectX::XMMatrixTranspose(projTransform);

    deviceContext->Unmap(perObjectTransformBuffer, 0);

    deviceContext->DrawIndexed(d3d11Sprite.getIndecesNum(), 0, 0);

    //

    worldTransform = DirectX::XMMatrixTranslation(-0.1f, -0.1f, 0.0f);
    worldTransform = DirectX::XMMatrixMultiply(scaleTransform, worldTransform);

    deviceContext->Map(perObjectTransformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

    mappedTransforms = (PerObjectTransforms*)(mappedSubresource.pData);
    mappedTransforms->worldTransformMatrix = DirectX::XMMatrixTranspose(worldTransform);
    mappedTransforms->viewTransformMatrix = DirectX::XMMatrixTranspose(viewTransform);
    mappedTransforms->projTransformMatrix = DirectX::XMMatrixTranspose(projTransform);

    deviceContext->Unmap(perObjectTransformBuffer, 0);

    deviceContext->DrawIndexed(d3d11Sprite.getIndecesNum(), 0, 0);

    swapChain->Present(0, 0);
}
