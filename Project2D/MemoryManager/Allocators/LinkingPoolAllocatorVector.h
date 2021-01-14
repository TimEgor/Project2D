#pragma once

#include <MemoryManager/Allocators/LinkingPoolAllocator.h>

#include <vector>

class Heap;

class LinkingPoolAllocatorVector final {
public:
	struct AllocationInfo final {
		void* allocationAddress;
		size_t allocatorIndex;
	};

private:
	std::vector<LinkingPoolAllocator> allocators;

	Heap* heap;

	size_t elementSize;
	size_t elementsNum;

	size_t maxEmptyAllocatorsNum;
	size_t emptyAllocatorsNum;

	LinkingPoolAllocator& createAllocator();
	void releaseAllocator(size_t index);
	bool checkAllocatorUseless(size_t index);
	bool checkAllocatorUseless(const LinkingPoolAllocator& allocator);

public:
	LinkingPoolAllocatorVector() = default;
	LinkingPoolAllocatorVector(const LinkingPoolAllocatorVector&) = delete;
	LinkingPoolAllocatorVector(LinkingPoolAllocatorVector && vector);
	~LinkingPoolAllocatorVector() { release(); }

	LinkingPoolAllocatorVector& operator=(const LinkingPoolAllocatorVector&) = delete;
	LinkingPoolAllocatorVector& operator=(LinkingPoolAllocatorVector && vector);
	LinkingPoolAllocator& operator[](size_t index);

	bool init(Heap * heap, size_t elementSize, size_t elementsNum, size_t maxEmptyAllocatorsNum = 1, size_t preInitAllocatorNum = 1);
	void release();

	void releaseUselessAllocators(bool releaseAllEmptyAllocators = false);

	AllocationInfo allocate();
	void deallocate(size_t allocatorIndex, void* address);
	void deallocate(const AllocationInfo & info);

	size_t size() const { return allocators.size(); }
	bool empty() const { return allocators.empty(); }
};