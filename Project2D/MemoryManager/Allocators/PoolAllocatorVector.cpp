#include "PoolAllocatorVector.h"

PoolAllocatorVector::PoolAllocatorVector(PoolAllocatorVector&& vector)
	: allocators(std::move(vector.allocators)), heap(vector.heap), elementSize(vector.elementSize), elementsNum(vector.elementsNum) {}

PoolAllocatorVector& PoolAllocatorVector::operator=(PoolAllocatorVector&& vector) {
    allocators = std::move(vector.allocators);

	heap = vector.heap;
    elementSize = vector.elementSize;
    elementsNum = vector.elementsNum;

    vector.elementSize = 0;
    vector.elementsNum = 0;

    return *this;
}

void PoolAllocatorVector::release() {
    elementSize = 0;
    elementsNum = 0;

    for (auto& allocator : allocators) {
        allocator.release();
    }

    allocators = std::vector<PoolAllocator>();
}

PoolAllocatorVector::AllocationInfo PoolAllocatorVector::allocate() {
	PoolAllocator* choosenAllocator = nullptr;

	for (auto& allocator : allocators) {
		if (!allocator.isFull()) {
			choosenAllocator = &allocator;
			break;
		}

		if (!allocator.isInit()) {
			allocator.init(heap, elementSize, elementsNum);
			choosenAllocator = &allocator;
			break;
		}
	}

	if (choosenAllocator == nullptr) {
		PoolAllocator newAllocator;
		newAllocator.init(heap, elementSize, elementsNum);
		allocators.push_back(std::move(newAllocator));
		choosenAllocator = &(*allocators.rbegin());
	}

	AllocationInfo info;
	info.allocationAddress = choosenAllocator->allocate();
	info.allocatorIndex = choosenAllocator - &allocators[0];

	return info;
}

void PoolAllocatorVector::deallocate(size_t allocatorIndex, void* address) {
	allocators[allocatorIndex].deallocate(address);
}

void PoolAllocatorVector::deallocate(const AllocationInfo& info) {
	allocators[info.allocatorIndex].deallocate(info.allocationAddress);
}
