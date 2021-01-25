#pragma once

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi.h>

#include <cstdint>

#define SWAP_CHAIN_BUFFER_COUNT 2

class D3D12 final {
private:
	ID3D12Device* device;
	IDXGIFactory* dxgiFactory;

	IDXGISwapChain* swapChain;

	ID3D12Fence* fence;
	uint64_t currentFenceVal;

	ID3D12GraphicsCommandList* commandList;
	ID3D12CommandAllocator* commandAllocator;
	ID3D12CommandQueue* commandQueue;

	ID3D12DescriptorHeap* rtvDescHeap;
	//ID3D12DescriptorHeap* dsvDescHeap;

	ID3D12Resource* swapChainBuffers[SWAP_CHAIN_BUFFER_COUNT];
	//ID3D12Resource* depthStencilBuffer;

	uint32_t rtvDescSize;
	uint32_t dsvDescSize;
	uint32_t cbv_srv_uavDescSize;

	uint32_t currentSwapChainBufferIndex;

	D3D12() : device(nullptr), dxgiFactory(nullptr), swapChain(nullptr),
		fence(nullptr), currentFenceVal(0), commandList(nullptr), commandAllocator(nullptr), commandQueue(nullptr),
		rtvDescHeap(nullptr), swapChainBuffers{},
		rtvDescSize(0), dsvDescSize(0), cbv_srv_uavDescSize(0),
		currentSwapChainBufferIndex(0) {}

	bool initSwapChain();
	bool initDescriptorHeaps();

public:
	~D3D12() { release(); }

	static D3D12& get();

	bool init();
	void release();

	void resize();

	void flushCommandQueue();

	ID3D12Device* getDevice() { return device; }
	IDXGIFactory* getDXGIFactory() { return dxgiFactory; }

	IDXGISwapChain* getSwapChain() { return swapChain; }
	ID3D12DescriptorHeap* getSwapChainRTVDescriptorHeap() { return rtvDescHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE getCurrentBackBufferRTVDescriptor();
	ID3D12Resource** getSwapChainBuffers() { return swapChainBuffers; }
	ID3D12Resource* getCurrentBackBuffer() { return swapChainBuffers[currentSwapChainBufferIndex]; }

	uint32_t getCurrentBackBufferIndex() { return currentSwapChainBufferIndex; }
	void increaseCurrentBackBufferIndex();

	ID3D12GraphicsCommandList* getCommandList() { return commandList; }
	ID3D12CommandAllocator* getCommandAllocator() { return commandAllocator; }
	ID3D12CommandQueue* getCommandQueue() { return commandQueue; }

	uint32_t getRTVDescriptorSize() { return rtvDescSize; }
	uint32_t getDSVDescriptorSize() { return dsvDescSize; }
	uint32_t getCBV_SRV_UAVDescriptorSize() { return cbv_srv_uavDescSize; }
};