#include "D3D12Renderer.h"

D3D12Renderer& D3D12Renderer::get() {
    static D3D12Renderer uniqueD3D12Renderer;
    return uniqueD3D12Renderer;
}

bool D3D12Renderer::init() {
    return true;
}

void D3D12Renderer::release() {
}

void D3D12Renderer::testDraw() {
    D3D12& d3d12 = D3D12::get();

    ID3D12CommandAllocator* commandListAllocator =  d3d12.getCommandAllocator(); 
    commandListAllocator->Reset();

    ID3D12GraphicsCommandList* commandList = d3d12.getCommandList();
    commandList->Reset(commandListAllocator, nullptr);

    D3D12_RESOURCE_BARRIER backBufferBarrier;
    backBufferBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    backBufferBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    backBufferBarrier.Transition.pResource = d3d12.getCurrentBackBuffer();
    backBufferBarrier.Transition.Subresource = 0;
    backBufferBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    backBufferBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    commandList->ResourceBarrier(1, &backBufferBarrier);

    D3D12_CPU_DESCRIPTOR_HANDLE currentRenderTargetViewDesc = d3d12.getCurrentBackBufferRTVDescriptor();

    float clearColor[] = { 0.0f, 0.0f, 0.5f, 1.0f };
    commandList->ClearRenderTargetView(currentRenderTargetViewDesc, clearColor, 0, nullptr);

    commandList->OMSetRenderTargets(1, &currentRenderTargetViewDesc, true, nullptr);

    backBufferBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    backBufferBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

    commandList->ResourceBarrier(1, &backBufferBarrier);

    commandList->Close();

    ID3D12CommandList* lists[] = { commandList };
    d3d12.getCommandQueue()->ExecuteCommandLists(1, lists);

    d3d12.getSwapChain()->Present(0, 0);

    d3d12.increaseCurrentBackBufferIndex();

    d3d12.flushCommandQueue();
}
