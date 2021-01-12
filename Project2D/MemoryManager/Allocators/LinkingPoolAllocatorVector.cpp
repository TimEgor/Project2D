#include "LinkingPoolAllocatorVector.h"

LinkingPoolAllocatorVector::LinkingPoolAllocatorVector(LinkingPoolAllocatorVector&& vector)
	: allocators(std::move(vector.allocators)), heap(vector.heap), elementSize(vector.elementSize), elementsNum(vector.elementsNum) {}

LinkingPoolAllocatorVector& LinkingPoolAllocatorVector::operator=(LinkingPoolAllocatorVector&& vector) {
    allocators = std::move(vector.allocators);

	heap = vector.heap;
    elementSize = vector.elementSize;
    elementsNum = vector.elementsNum;

    vector.elementSize = 0;
    vector.elementsNum = 0;

    return *this;
}

LinkingPoolAllocator& LinkingPoolAllocatorVector::operator[](size_t index) {
	return allocators[index];
}

bool LinkingPoolAllocatorVector::init(Heap* _heap, size_t _elementSize, size_t _elementsNum) {
	heap = _heap;
	elementSize = _elementSize;
	elementsNum = _elementsNum;

	return true;
}

void LinkingPoolAllocatorVector::release() {
    elementSize = 0;
    elementsNum = 0;

    for (auto& allocator : allocators) {
        allocator.release();
    }

    allocators = std::vector<LinkingPoolAllocator>();
}

LinkingPoolAllocatorVector::AllocationInfo LinkingPoolAllocatorVector::allocate() {
	LinkingPoolAllocator* choosenAllocator = nullptr;

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
		LinkingPoolAllocator newAllocator;
		newAllocator.init(heap, elementSize, elementsNum);
		allocators.push_back(std::move(newAllocator));
		choosenAllocator = &(*allocators.rbegin());
	}

	AllocationInfo info;
	info.allocationAddress = choosenAllocator->allocate();
	info.allocatorIndex = choosenAllocator - &allocators[0];

	return info;
}

void LinkingPoolAllocatorVector::deallocate(size_t allocatorIndex, void* address) {
	allocators[allocatorIndex].deallocate(address);
}

void LinkingPoolAllocatorVector::deallocate(const AllocationInfo& info) {
	allocators[info.allocatorIndex].deallocate(info.allocationAddress);
}
