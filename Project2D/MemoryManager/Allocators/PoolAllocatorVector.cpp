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

PoolAllocator& PoolAllocatorVector::operator[](size_t index) {
	return allocators[index];
}

bool PoolAllocatorVector::init(Heap* _heap, size_t _elementSize, size_t _elementsNum, size_t _maxEmptyAllocatorsNum, size_t preInitAllocatorNum) {
	heap = _heap;
	elementSize = _elementSize;
	elementsNum = _elementsNum;

	maxEmptyAllocatorsNum = _maxEmptyAllocatorsNum;
	emptyAllocatorsNum = 0;

	for (size_t i = 0; i < preInitAllocatorNum; ++i) {
		createAllocator();
	}

	return true;
}

void PoolAllocatorVector::release() {
    elementSize = 0;
    elementsNum = 0;

	maxEmptyAllocatorsNum = 0;
	emptyAllocatorsNum = 0;

    for (auto& allocator : allocators) {
        allocator.release();
    }

    allocators = std::vector<PoolAllocator>();
}

PoolAllocator& PoolAllocatorVector::createAllocator() {
	PoolAllocator newAllocator;
	newAllocator.init(heap, elementSize, elementsNum);
	allocators.push_back(std::move(newAllocator));

	++emptyAllocatorsNum;

	return *(allocators.rbegin());
}

void PoolAllocatorVector::releaseAllocator(size_t index) {
	auto indexIter = allocators.begin();
	std::advance(indexIter, index);

	if (indexIter->isEmpty()) {
		--emptyAllocatorsNum;
	}

	indexIter->release();
	std::iter_swap(indexIter, allocators.rbegin());
	allocators.pop_back();
}

bool PoolAllocatorVector::checkAllocatorUseless(size_t index) {
	return checkAllocatorUseless(allocators[index]);
}

bool PoolAllocatorVector::checkAllocatorUseless(const PoolAllocator& allocator) {
	if (allocator.isEmpty() && emptyAllocatorsNum > maxEmptyAllocatorsNum) {
		return true;
	}

	return false;
}

void PoolAllocatorVector::releaseUselessAllocators(bool releaseAllEmptyAllocators) {
	for (size_t i = 0; i < allocators.size(); ++i) {
		while (checkAllocatorUseless(i)) {
			releaseAllocator(i);
		}
	}
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
		choosenAllocator = &createAllocator();
	}

	if (choosenAllocator->isEmpty()) {
		--emptyAllocatorsNum;
	}

	AllocationInfo info;
	info.allocationAddress = choosenAllocator->allocate();
	info.allocatorIndex = choosenAllocator - &allocators[0];

	return info;
}

void PoolAllocatorVector::deallocate(size_t allocatorIndex, void* address) {
	PoolAllocator& allocator = allocators[allocatorIndex];
	allocator.deallocate(address);

	if (allocator.isEmpty()) {
		++emptyAllocatorsNum;

		if (checkAllocatorUseless(allocator)) {
			releaseAllocator(allocatorIndex);
		}
	}
}

void PoolAllocatorVector::deallocate(const AllocationInfo& info) {
	deallocate(info.allocatorIndex, info.allocationAddress);
}
