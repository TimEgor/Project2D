#include "LinkingPoolAllocatorVector.h"

LinkingPoolAllocatorVector::LinkingPoolAllocatorVector(LinkingPoolAllocatorVector&& vector)
	: allocators(std::move(vector.allocators)), heap(vector.heap), elementSize(vector.elementSize), elementsNum(vector.elementsNum) {
}

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

bool LinkingPoolAllocatorVector::init(Heap* _heap, size_t _elementSize, size_t _elementsNum, size_t _maxEmptyAllocatorsNum, size_t preInitAllocatorNum) {
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

void LinkingPoolAllocatorVector::release() {
	elementSize = 0;
	elementsNum = 0;

	maxEmptyAllocatorsNum = 0;
	emptyAllocatorsNum = 0;

	for (auto& allocator : allocators) {
		allocator.release();
	}

	allocators = std::vector<LinkingPoolAllocator>();
}

LinkingPoolAllocator& LinkingPoolAllocatorVector::createAllocator() {
	LinkingPoolAllocator newAllocator;
	newAllocator.init(heap, elementSize, elementsNum);
	allocators.push_back(std::move(newAllocator));

	++emptyAllocatorsNum;

	return *(allocators.rbegin());
}

void LinkingPoolAllocatorVector::releaseAllocator(size_t index) {
	auto indexIter = allocators.begin();
	std::advance(indexIter, index);

	if (indexIter->isEmpty()) {
		--emptyAllocatorsNum;
	}

	indexIter->release();
	std::iter_swap(indexIter, allocators.rbegin());
	allocators.pop_back();
}

bool LinkingPoolAllocatorVector::checkAllocatorUseless(size_t index) {
	return checkAllocatorUseless(allocators[index]);
}

bool LinkingPoolAllocatorVector::checkAllocatorUseless(const LinkingPoolAllocator& allocator) {
	if (allocator.isEmpty() && emptyAllocatorsNum > maxEmptyAllocatorsNum) {
		return true;
	}

	return false;
}

void LinkingPoolAllocatorVector::releaseUselessAllocators(bool releaseAllEmptyAllocators) {
	for (size_t i = 0; i < allocators.size(); ++i) {
		while (checkAllocatorUseless(i)) {
			releaseAllocator(i);
		}
	}
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

void LinkingPoolAllocatorVector::deallocate(size_t allocatorIndex, void* address) {
	LinkingPoolAllocator& allocator = allocators[allocatorIndex];
	allocator.deallocate(address);

	if (allocator.isEmpty()) {
		++emptyAllocatorsNum;

		if (checkAllocatorUseless(allocator)) {
			releaseAllocator(allocatorIndex);
		}
	}
}

void LinkingPoolAllocatorVector::deallocate(const AllocationInfo& info) {
	deallocate(info.allocatorIndex, info.allocationAddress);
}
