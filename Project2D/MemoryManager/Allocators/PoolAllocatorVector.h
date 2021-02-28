#pragma once

#include <vector>
#include <unordered_map>
#include <cassert>

class Heap;

typedef size_t AllocatorID;

template <typename AllocatorType>
class PoolAllocatorVector final {
public:
	struct AllocationInfo final {
		void* allocationAddress;
		AllocatorID allocatorID;
	};

private:
	struct VectorNode final {
		AllocatorType allocator;
		AllocatorID id;

		VectorNode() = default;
		VectorNode(const VectorNode&) = delete;
		VectorNode(VectorNode&& node)
			: allocator(std::move(node.allocator)), id(node.id) {}

		VectorNode& operator=(const VectorNode&) = delete;
		VectorNode& operator=(VectorNode&& node) {
			allocator = std::move(node.allocator);
			id = node.id;

			return *this;
		}
	};

	void swap(PoolAllocatorVector::VectorNode& node1, PoolAllocatorVector::VectorNode& node2) {
		VectorNode tmpNode = std::move(node1);
		node1 = std::move(node2);
		node2 = std::move(tmpNode);
	}

	std::vector<VectorNode> allocators;
	std::unordered_map<size_t, size_t> indeces;

	Heap* heap;

	size_t elementSize;
	size_t elementsNum;

	size_t maxEmptyAllocatorsNum;
	size_t emptyAllocatorsNum;

	AllocatorID nextFreeAllocatorID;

	VectorNode& createAllocator() {
		AllocatorType newAllocator;
		newAllocator.init(heap, elementSize, elementsNum);

		allocators.emplace_back();
		VectorNode& newNode = *allocators.rbegin();
		newNode.allocator = std::move(newAllocator);
		newNode.id = nextFreeAllocatorID;

		indeces.emplace(nextFreeAllocatorID, allocators.size() - 1);

		++emptyAllocatorsNum;
		++nextFreeAllocatorID;

		return *(allocators.rbegin());
	}

	void releaseAllocatorByIndex(size_t index) {
		auto indexIter = allocators.begin();
		std::advance(indexIter, index);

		VectorNode& node = *indexIter;

		if (node.allocator.isEmpty()) {
			--emptyAllocatorsNum;
		}

		node.allocator.release();

		indeces.erase(node.id);

		if (allocators.size() != 1) {
			swap(*indexIter, *allocators.rbegin());

			indeces[node.id] = index;
		}

		allocators.pop_back();

		if (allocators.capacity() >= allocators.size() * 2) {
			allocators.shrink_to_fit();
		}
	}

	void releaseAllocatorByID(AllocatorID id) {
		auto idIter = indeces.find(id);
		assert(idIter != indeces.end());

		releaseAllocatorByIndex(idIter->second);
	}

	bool checkAllocatorUseless(size_t index) {
		return checkAllocatorUseless(allocators[index]);
	}

	bool checkAllocatorUseless(const AllocatorType& allocator) {
		if (allocator.isEmpty() && emptyAllocatorsNum > maxEmptyAllocatorsNum) {
			return true;
		}

		return false;
	}

public:
	PoolAllocatorVector() = default;
	PoolAllocatorVector(const PoolAllocatorVector&) = delete;
	PoolAllocatorVector(PoolAllocatorVector&& vector)
		: allocators(std::move(vector.allocators)), heap(vector.heap),
		elementSize(vector.elementSize), elementsNum(vector.elementsNum) {}

	~PoolAllocatorVector() { release(); }

	PoolAllocatorVector& operator=(const PoolAllocatorVector&) = delete;
	PoolAllocatorVector& operator=(PoolAllocatorVector&& vector) {
		allocators = std::move(vector.allocators);

		heap = vector.heap;
		elementSize = vector.elementSize;
		elementsNum = vector.elementsNum;

		vector.elementSize = 0;
		vector.elementsNum = 0;

		return *this;
	}

	AllocatorType& operator[](size_t index) {
		return allocators[index].allocator;
	}

	bool init(Heap* allocatorHeap, size_t allocatorElementSize, size_t allocatorElementsNum, size_t maxEmptyStoringAllocatorsNum = 1, size_t preInitAllocatorNum = 1) {
		heap = allocatorHeap;
		elementSize = allocatorElementSize;
		elementsNum = allocatorElementsNum;

		maxEmptyAllocatorsNum = maxEmptyStoringAllocatorsNum;
		emptyAllocatorsNum = 0;

		for (size_t i = 0; i < preInitAllocatorNum; ++i) {
			createAllocator();
		}

		return true;
	}

	void release() {
		elementSize = 0;
		elementsNum = 0;

		maxEmptyAllocatorsNum = 0;
		emptyAllocatorsNum = 0;

		for (auto& allocatorNode : allocators) {
			allocatorNode.allocator.release();
		}

		allocators = std::vector<VectorNode>();
	}

	size_t getAllocatorIndexByID(AllocatorID id) {
		auto idIter = indeces.find(id);
		assert(idIter != indeces.end());

		return idIter->second;
	}

	void releaseUselessAllocators(bool releaseAllEmptyAllocators = false) {
		for (size_t i = 0; i < allocators.size(); ++i) {
			while (checkAllocatorUseless(i)) {
				releaseAllocatorByIndex(i);
			}
		}
	}

	AllocationInfo allocate() {
		VectorNode* choosenVectorNode = nullptr;

		for (auto& node : allocators) {
			AllocatorType& allocator = node.allocator;
			if (!(allocator.isFull())) {
				choosenVectorNode = &node;
				break;
			}

			if (!allocator.isInit()) {
				allocator.init(heap, elementSize, elementsNum);
				choosenVectorNode = &node;
				break;
			}
		}

		if (choosenVectorNode == nullptr) {
			choosenVectorNode = &createAllocator();
		}

		AllocatorType& allocator = choosenVectorNode->allocator;

		if (allocator.isEmpty()) {
			--emptyAllocatorsNum;
		}

		AllocationInfo info;
		info.allocationAddress = allocator.allocate();
		info.allocatorID = choosenVectorNode->id;

		return info;
	}

	void deallocate(AllocatorID allocatorID, void* address) {
		auto idIter = indeces.find(allocatorID);
		assert(idIter != indeces.end());

		size_t nodeIndex = idIter->second;

		AllocatorType& allocator = allocators[nodeIndex].allocator;
		allocator.deallocate(address);

		if (allocator.isEmpty()) {
			++emptyAllocatorsNum;

			if (checkAllocatorUseless(allocator)) {
				releaseAllocatorByIndex(nodeIndex);
			}
		}
	}

	void deallocate(const AllocationInfo& info) {
		deallocate(info.allocatorID, info.allocationAddress);
	}

	size_t size() const { return allocators.size(); }
	bool empty() const { return allocators.empty(); }
};