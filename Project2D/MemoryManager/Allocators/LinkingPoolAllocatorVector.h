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

public:
	LinkingPoolAllocatorVector() = default;
	LinkingPoolAllocatorVector(const LinkingPoolAllocatorVector&) = delete;
	LinkingPoolAllocatorVector(LinkingPoolAllocatorVector&& vector);
	~LinkingPoolAllocatorVector() { release(); }

	LinkingPoolAllocatorVector& operator=(const LinkingPoolAllocatorVector&) = delete;
	LinkingPoolAllocatorVector& operator=(LinkingPoolAllocatorVector&& vector);
	LinkingPoolAllocator& operator[](size_t index);

	bool init(Heap* heap, size_t elementSize, size_t elementsNum);
	void release();

	AllocationInfo allocate();
	void deallocate(size_t allocatorIndex, void* address);
	void deallocate(const AllocationInfo& info);

	size_t size() const { allocators.size(); }
	bool empty() const { allocators.empty(); }
};