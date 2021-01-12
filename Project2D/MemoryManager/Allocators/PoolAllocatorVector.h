#pragma once

#include <MemoryManager/Allocators/PoolAllocator.h>

#include <vector>

class Heap;

class PoolAllocatorVector final {
public:
	struct AllocationInfo final {
		void* allocationAddress;
		size_t allocatorIndex;
	};

private:
	std::vector<PoolAllocator> allocators;

	Heap* heap;

	size_t elementSize;
	size_t elementsNum;

public:
	PoolAllocatorVector() = default;
	PoolAllocatorVector(const PoolAllocatorVector&) = delete;
	PoolAllocatorVector(PoolAllocatorVector&& vector);
	~PoolAllocatorVector() { release(); }

	PoolAllocatorVector& operator=(const PoolAllocatorVector&) = delete;
	PoolAllocatorVector& operator=(PoolAllocatorVector&& vector);
	PoolAllocator& operator[](size_t index);

	bool init(Heap* heap, size_t elementSize, size_t elementsNum);
	void release();

	AllocationInfo allocate();
	void deallocate(size_t allocatorIndex, void* address);
	void deallocate(const AllocationInfo& info);

	size_t size() const { allocators.size(); }
	bool empty() const { allocators.empty(); }
};