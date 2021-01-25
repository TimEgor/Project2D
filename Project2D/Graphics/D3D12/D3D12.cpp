#include "D3D12.h"

#include <UserInterfaces/UserInterfaces.h>

D3D12& D3D12::get() {
    static D3D12 uniqueD3D12;
    return uniqueD3D12;
}

bool D3D12::init() {
    HRESULT result;

    result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, __uuidof(device), (void**)(&device));
    if (FAILED(result)) {
        return false;
    }

    result = CreateDXGIFactory(__uuidof(dxgiFactory), (void**)(&dxgiFactory));
    if (FAILED(result)) {
        release();
        return false;
    }

    result = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(fence), (void**)(&fence));
    if (FAILED(result)) {
        release();
        return false;
    }

    D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    result = device->CreateCommandQueue(&commandQueueDesc, __uuidof(commandQueue), (void**)(&commandQueue));
    if (FAILED(result)) {
        release();
        return false;
    }

    result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(commandAllocator), (void**)(&commandAllocator));
    if (FAILED(result)) {
        release();
        return false;
    }

    result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, __uuidof(commandList), (void**)(&commandList));
    if (FAILED(result)) {
        release();
        return false;
    }
    commandList->Close();

    if (!initSwapChain()) {
        release();
        return false;
    }

    if (!initDescriptorHeaps()) {
        release();
        return false;
    }

    for (size_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
        swapChainBuffers[i] = nullptr;
    }

    depthStencilBuffer = nullptr;

    resize();

    return true;
}

bool D3D12::initSwapChain() {
    UserInterfaces& userInterfaces = UserInterfaces::get();

    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferDesc.Width = 800;
    swapChainDesc.BufferDesc.Height = 800;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDesc.Windowed = true;
    swapChainDesc.OutputWindow = userInterfaces.getMainWindow();

    HRESULT result = dxgiFactory->CreateSwapChain(commandQueue, &swapChainDesc, &swapChain);
    if (FAILED(result)) {
        return false;
    }

    return true;
}

bool D3D12::initDescriptorHeaps() {
    rtvDescSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    dsvDescSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    cbv_srv_uavDescSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    HRESULT result;

    D3D12_DESCRIPTOR_HEAP_DESC descViewDesc{};
    descViewDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    descViewDesc.NodeMask = 0;

    descViewDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    descViewDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
    result = device->CreateDescriptorHeap(&descViewDesc, __uuidof(rtvDescHeap), (void**)(&rtvDescHeap));
    if (FAILED(result)) {
        return false;
    }

    descViewDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    descViewDesc.NumDescriptors = 1;
    result = device->CreateDescriptorHeap(&descViewDesc, __uuidof(dsvDescHeap), (void**)(&dsvDescHeap));
    if (FAILED(result)) {
        return false;
    }

    return true;
}

void D3D12::release() {
    if (device) {
        device->Release();
        device = nullptr;
    }

    if (dxgiFactory) {
        dxgiFactory->Release();
        dxgiFactory = nullptr;
    }

    if (fence) {
        fence->Release();
        fence = nullptr;
    }

    if (commandQueue) {
        commandQueue->Release();
        commandQueue = nullptr;
    }

    if (commandAllocator) {
        commandAllocator->Release();
        commandAllocator = nullptr;
    }

    if (commandList) {
        commandList->Release();
        commandList = nullptr;
    }

    if (swapChain) {
        swapChain->Release();
        swapChain = nullptr;
    }

    if (rtvDescHeap) {
        rtvDescHeap->Release();
        rtvDescHeap = nullptr;
    }

    if (dsvDescHeap) {
        dsvDescHeap->Release();
        dsvDescHeap = nullptr;
    }

    rtvDescSize = 0;
    dsvDescSize = 0;
    cbv_srv_uavDescSize = 0;

    for (size_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
        if (swapChainBuffers[i]) {
            swapChainBuffers[i]->Release();
            swapChainBuffers[i] = nullptr;
        }
    }

    if (depthStencilBuffer) {
        depthStencilBuffer->Release();
        depthStencilBuffer = nullptr;
    }

    currentSwapChainBufferIndex = 0;
}

void D3D12::resize() {
    flushCommandQueue();

    if (commandList) {
        commandList->Reset(commandAllocator, nullptr);
    }

    for (size_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
        if (swapChainBuffers[i]) {
            swapChainBuffers[i]->Release();
            swapChainBuffers[i] = nullptr;
        }
    }

    if (depthStencilBuffer) {
        depthStencilBuffer->Release();
        depthStencilBuffer = nullptr;
    }

    swapChain->ResizeBuffers(SWAP_CHAIN_BUFFER_COUNT, 800, 800, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

    currentSwapChainBufferIndex = 0;

    for (size_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
        swapChain->GetBuffer(i, __uuidof(swapChainBuffers[i]), (void**)(&swapChainBuffers[i]));
        device->CreateRenderTargetView(swapChainBuffers[i], nullptr,
            D3D12_CPU_DESCRIPTOR_HANDLE{ rtvDescHeap->GetCPUDescriptorHandleForHeapStart().ptr + rtvDescSize * i });
    }

    D3D12_RESOURCE_DESC dsvBufferDesc{};
    dsvBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    dsvBufferDesc.Alignment = 0;
    dsvBufferDesc.Width = 800;
    dsvBufferDesc.Height = 800;
    dsvBufferDesc.DepthOrArraySize = 1;
    dsvBufferDesc.MipLevels = 1;
    dsvBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    dsvBufferDesc.SampleDesc.Count = 1;
    dsvBufferDesc.SampleDesc.Quality = 0;
    dsvBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    dsvBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE clearVal;
    clearVal.Format = DXGI_FORMAT_R24G8_TYPELESS;
    clearVal.DepthStencil.Depth = 1.0f;
    clearVal.DepthStencil.Stencil = 0;

    D3D12_HEAP_PROPERTIES heapProperties = device->GetCustomHeapProperties(0, D3D12_HEAP_TYPE_DEFAULT);

    device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
        &dsvBufferDesc, D3D12_RESOURCE_STATE_COMMON, &clearVal, __uuidof(depthStencilBuffer), (void**)(&depthStencilBuffer));

    device->CreateDepthStencilView(depthStencilBuffer, nullptr, dsvDescHeap->GetCPUDescriptorHandleForHeapStart());

    D3D12_RESOURCE_BARRIER dsBarrier;
    dsBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    dsBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    dsBarrier.Transition.pResource = depthStencilBuffer;
    dsBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    dsBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
    dsBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;

    commandList->ResourceBarrier(1, &dsBarrier);

    D3D12_VIEWPORT viewPort;
    viewPort.Width = 800.0f;
    viewPort.Height = 800.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.MinDepth = 0.0f;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;

    commandList->RSSetViewports(1, &viewPort);

    commandList->Close();
    ID3D12CommandList* list[] = { commandList };
    commandQueue->ExecuteCommandLists(1, list);

    flushCommandQueue();
}

void D3D12::flushCommandQueue() {
    ++currentFenceVal;

    fence->Signal(currentFenceVal);

    if (fence->GetCompletedValue() < currentFenceVal) {
        HANDLE waitEvent = CreateEventEx(NULL, "", false, EVENT_ALL_ACCESS);

        fence->SetEventOnCompletion(currentFenceVal, waitEvent);
        WaitForSingleObject(waitEvent, INFINITE);
        CloseHandle(waitEvent);
    }
}
