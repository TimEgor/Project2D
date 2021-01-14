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

	size_t maxEmptyAllocatorsNum;
	size_t emptyAllocatorsNum;

	PoolAllocator& createAllocator();
	void releaseAllocator(size_t index);
	bool checkAllocatorUseless(size_t index);
	bool checkAllocatorUseless(const PoolAllocator &allocator);

public:
	PoolAllocatorVector() = default;
	PoolAllocatorVector(const PoolAllocatorVector&) = delete;
	PoolAllocatorVector(PoolAllocatorVector&& vector);
	~PoolAllocatorVector() { release(); }

	PoolAllocatorVector& operator=(const PoolAllocatorVector&) = delete;
	PoolAllocatorVector& operator=(PoolAllocatorVector&& vector);
	PoolAllocator& operator[](size_t index);

	bool init(Heap* heap, size_t elementSize, size_t elementsNum, size_t maxEmptyAllocatorsNum = 1, size_t preInitAllocatorNum = 1);
	void release();

	void releaseUselessAllocators(bool releaseAllEmptyAllocators = false);

	AllocationInfo allocate();
	void deallocate(size_t allocatorIndex, void* address);
	void deallocate(const AllocationInfo& info);

	size_t size() const { return allocators.size(); }
	bool empty() const { return allocators.empty(); }
};