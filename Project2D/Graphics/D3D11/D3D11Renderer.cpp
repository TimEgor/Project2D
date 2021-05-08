#include "D3D11Renderer.h"

#include <Graphics/D3D11/D3D11Sprite.h>
#include <Graphics/D3D11/Resources/D3D11MaterialResource.h>
#include <Graphics/D3D11/Resources/D3D11ShaderResource.h>
#include <Graphics/D3D11/Resources/D3D11TextureResource.h>
#include <Graphics/D3D11/D3D11Verteces.h>
#include <Graphics/D3D11/D3D11SpriteBatchManager.h>
#include <Graphics/Scene.h>
#include <ResourceManager/ResourceManager.h>

D3D11Renderer::D3D11Renderer() : renderTargetView(nullptr),
    depthStencilBuffer(nullptr), depthStencilView(nullptr),
    rasterizerState(nullptr),
    perObjectTransformBuffer(nullptr), spriteSamplerState(nullptr) {}

D3D11Renderer& D3D11Renderer::get() {
    static D3D11Renderer uniqueD3D11Renderer;
    return uniqueD3D11Renderer;
}

bool D3D11Renderer::init() {
    D3D11& d3d11 = D3D11::get();

    ID3D11Device* device = d3d11.getDevice();
    ID3D11DeviceContext* deviceContext = d3d11.getDeviceContext();
    IDXGISwapChain* swapChain = d3d11.getSwapChain();

    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(backBuffer), (void**)(&backBuffer));
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);

    D3D11_TEXTURE2D_DESC dsBufferDesc{};
    dsBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsBufferDesc.Width = 800;
    dsBufferDesc.Height = 800;
    dsBufferDesc.MipLevels = 0;
    dsBufferDesc.ArraySize = 1;
    dsBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    dsBufferDesc.CPUAccessFlags = 0;
    dsBufferDesc.MipLevels = 1;
    dsBufferDesc.SampleDesc.Count = 1;
    dsBufferDesc.SampleDesc.Quality = 0;
    dsBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    device->CreateTexture2D(&dsBufferDesc, nullptr, &depthStencilBuffer);
    device->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView);

    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = true;

    device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

    D3D11_BUFFER_DESC perObjectTransformBufferDesc{};
    perObjectTransformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    perObjectTransformBufferDesc.ByteWidth = sizeof(PerObjectTransforms);
    perObjectTransformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    perObjectTransformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    perObjectTransformBufferDesc.MiscFlags = 0;
    perObjectTransformBufferDesc.StructureByteStride = 0;

    device->CreateBuffer(&perObjectTransformBufferDesc, nullptr, &perObjectTransformBuffer);

    D3D11_SAMPLER_DESC spriteSamplerDesc{};
    spriteSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    spriteSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    spriteSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    spriteSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    spriteSamplerDesc.MaxAnisotropy = 1;
    spriteSamplerDesc.MipLODBias = 0.0f;
    spriteSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    spriteSamplerDesc.MinLOD = -FLT_MAX;
    spriteSamplerDesc.MaxLOD = FLT_MAX;

    device->CreateSamplerState(&spriteSamplerDesc, &spriteSamplerState);

    D3D11_BLEND_DESC spriteBlendDesc{};
    spriteBlendDesc.IndependentBlendEnable = true;
    spriteBlendDesc.RenderTarget[0].BlendEnable = true;
    spriteBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    spriteBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    spriteBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    spriteBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    spriteBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    spriteBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    spriteBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    spriteBlendDesc.AlphaToCoverageEnable = false;

    device->CreateBlendState(&spriteBlendDesc, &spriteBlendState);

    D3D11Sprite::get().init();

    viewport.Width = 800.0f;
    viewport.Height = 800.0f;
    viewport.MaxDepth = 1.0f;
    viewport.MinDepth = 0.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    currentMaterialID = 0;
    currentSpriteID = 0;

    spriteBatchManager = new D3D11SpriteBatchManager();

    return true;
}

void D3D11Renderer::release() {
    D3D11ObjectRelease(renderTargetView);

    D3D11ObjectRelease(depthStencilBuffer);
    D3D11ObjectRelease(depthStencilView);

    D3D11ObjectRelease(rasterizerState);

    D3D11ObjectRelease(perObjectTransformBuffer);
    D3D11ObjectRelease(spriteSamplerState);

    if (spriteBatchManager) {
        delete spriteBatchManager;
        spriteBatchManager = nullptr;
    }
}

void D3D11Renderer::prepareViewTransformMatrix(DirectX::XMMATRIX& matrix) {
    DirectX::XMVECTOR position{ 0.0f, 0.0f, -5.0f };
    DirectX::XMVECTOR direction{ 0.0f, 0.0f, 1.0f };
    DirectX::XMVECTOR up{ 0.0f, 1.0f, 0.0f };
    matrix = DirectX::XMMatrixLookToLH(position, direction, up);
}

void D3D11Renderer::prepareSceneProjTransformMatrix(DirectX::XMMATRIX& matrix) {
    float ratio = atanf(45.0f / 2.0f) * 2.0f;
    float aspect = 1.0f;
    float z = 1.0f;

    float x = ratio * z;
    float y = ratio * z * aspect;

    matrix = DirectX::XMMatrixOrthographicOffCenterLH(-x, x, -y, y, 0.0f, 1000.0f);
}

void D3D11Renderer::prepareCanvasProjTransformMatrix(DirectX::XMMATRIX& matrix) {
    matrix = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 800.0f, -800.0f, 0.0f, 0.0f, 1000.0f);
}

void D3D11Renderer::beginDrawing() {
    D3D11& d3d11 = D3D11::get();
    ID3D11DeviceContext* deviceContext = d3d11.getDeviceContext();

    float clearVal[4] = { 0.0f, 0.0f, 0.5f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearVal);
    deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void D3D11Renderer::endDrawing() {
    IDXGISwapChain* swapChain = D3D11::get().getSwapChain();
    swapChain->Present(0, 0);
}

void D3D11Renderer::changeMaterial(ResourceReference materialResource, ID3D11DeviceContext* context) {
    D3D11MaterialResource material = materialResource.getResource<D3D11MaterialResource>();

    D3D11VertexShaderResource vertexShader = material.getVertexShaderResource().getResource<D3D11VertexShaderResource>();
    context->VSSetShader(vertexShader.getVertexShader(), nullptr, 0);
    context->IASetInputLayout(vertexShader.getInputLayout());

    D3D11PixelShaderResource pixelShader = material.getPixelShaderResource().getResource<D3D11PixelShaderResource>();
    context->PSSetShader(pixelShader.getPixelShader(), nullptr, 0);

    currentMaterialID = materialResource.getResourceID();
}

void D3D11Renderer::changeSprite(ResourceReference spriteResource, ID3D11DeviceContext* context) {
    ID3D11ShaderResourceView* spriteShaderResourceView = spriteResource.getResource<D3D11TextureResource>().getShaderResoruceView();
    context->PSSetShaderResources(0, 1, &spriteShaderResourceView);

    currentSpriteID = spriteResource.getResourceID();
}

void D3D11Renderer::drawSprite(const RenderingOrderNode& node, size_t indexPos, size_t indexCount, ID3D11DeviceContext* deviceContext) {
    if (node.materialResource.getResourceID() != currentMaterialID) {
        changeMaterial(node.materialResource, deviceContext);
    }

    if (node.spriteResource.getResourceID() != currentSpriteID) {
        changeSprite(node.spriteResource, deviceContext);
    }

    deviceContext->DrawIndexed(indexCount, indexPos, 0);
}

void D3D11Renderer::draw(RenderingData data) {
    D3D11& d3d11 = D3D11::get();

    ID3D11Device* device = d3d11.getDevice();
    ID3D11DeviceContext* deviceContext = d3d11.getDeviceContext();

    float blendFactor[4] = { 1.0f };

    deviceContext->OMSetBlendState(spriteBlendState, blendFactor, 0xffffffff);
    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
    deviceContext->RSSetState(rasterizerState);
    deviceContext->RSSetViewports(1, &viewport);
    deviceContext->PSSetSamplers(0, 1, &spriteSamplerState);
    deviceContext->VSSetConstantBuffers(1, 1, &perObjectTransformBuffer);

    DirectX::XMMATRIX projTransformMatrix;
    DirectX::XMMATRIX viewTransformMatrix;
    prepareViewTransformMatrix(viewTransformMatrix);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Scene* scene = data.getScene();

    PerObjectTransforms* mappedTransforms;
    D3D11_MAPPED_SUBRESOURCE mappedSubresource{};

    size_t indexPos = 0;
    size_t indexCount = 6;

    ResourceID currentBatchMaterialID = 0;
    ResourceID currentBatchTextureID = 0;

    //

    prepareSceneProjTransformMatrix(projTransformMatrix);

    deviceContext->Map(perObjectTransformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

    mappedTransforms = (PerObjectTransforms*)(mappedSubresource.pData);
    mappedTransforms->viewTransformMatrix = DirectX::XMMatrixTranspose(viewTransformMatrix);
    mappedTransforms->projTransformMatrix = DirectX::XMMatrixTranspose(projTransformMatrix);

    deviceContext->Unmap(perObjectTransformBuffer, 0);

    RenderingOrder* order = data.getSceneRedneringOrder();
    size_t orderSize = order->size();

    if (orderSize == 0) {
        return;
    }

    spriteBatchManager->prepareRenderingData(&(*order)[0], orderSize);

    const std::vector<D3D11SpriteBatch>& batches = spriteBatchManager->getBatches();
    size_t batchesCount = spriteBatchManager->getPreparingBatchesCount();

    for (size_t i = 0; i < batchesCount; ++i) {
        const D3D11SpriteBatch& batch = batches[i];

        ID3D11Buffer* vertecesBuffer = batch.getVertecesBuffer();
        UINT strides = sizeof(D3D11SpriteVertex);
        UINT offsets = 0;

        deviceContext->IASetVertexBuffers(0, 1, &vertecesBuffer, &strides, &offsets);
        deviceContext->IASetIndexBuffer(batch.getIndecesBuffer(), DXGI_FORMAT_R16_UINT, 0);
        for (size_t i = 0; i < orderSize; ++i) {
            const RenderingOrderNode& node = (*order)[i];

            if (node.materialResource.getResourceID() != currentBatchMaterialID || node.spriteResource.getResourceID() != currentBatchTextureID) {
                if (i != 0) {
                    drawSprite(node, indexPos, indexCount, deviceContext);

                    indexPos += indexCount;
                    indexCount = 6;
                }

                currentBatchMaterialID = node.materialResource.getResourceID();
                currentBatchTextureID = node.spriteResource.getResourceID();
            }
            else {
                indexCount += 6;
            }
        }

        drawSprite((*order)[orderSize - 1], indexPos, indexCount, deviceContext);
    }

    //

    /*prepareCanvasProjTransformMatrix(projTransformMatrix);

    deviceContext->Map(perObjectTransformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

    mappedTransforms = (PerObjectTransforms*)(mappedSubresource.pData);
    mappedTransforms->viewTransformMatrix = DirectX::XMMatrixTranspose(viewTransformMatrix);
    mappedTransforms->projTransformMatrix = DirectX::XMMatrixTranspose(projTransformMatrix);

    deviceContext->Unmap(perObjectTransformBuffer, 0);

    order = data.getCanvasRedneringOrder();
    orderSize = order->size();
    for (size_t i = 0; i < orderSize; ++i) {
        const RenderingOrderNode& node = (*order)[i];

        if (node.materialResource.getResourceID() != currentMaterialID) {
            changeMaterial(node.materialResource, deviceContext);
        }

        if (node.spriteResource.getResourceID() != currentSpriteID) {
            changeSprite(node.spriteResource, deviceContext);
        }

        deviceContext->DrawIndexed(d3d11Sprite.getIndecesNum(), 0, 0);
    }*/
}
