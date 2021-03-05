#pragma once

#include <cstdint>

class Heap;

class ArrayPoolAllocator final {
private:
	Heap* heap;

	void* buckets;

	size_t usingBucketsNum;

	size_t bucketSize;
	size_t bucketsNum;

public:
	ArrayPoolAllocator();
	ArrayPoolAllocator(const ArrayPoolAllocator&) = delete;
	ArrayPoolAllocator(ArrayPoolAllocator&& allocator);
	~ArrayPoolAllocator() { release(); }

	ArrayPoolAllocator& operator=(const ArrayPoolAllocator&) = delete;
	ArrayPoolAllocator& operator=(ArrayPoolAllocator&& allocator);
	void* operator[](size_t index);

	template <typename T>
	T& getElement(size_t index) {
		return *(T*)((uint8_t*)(buckets) + index * bucketSize);
	}

	template <typename T>
	const T& getElement(size_t index) const {
		return *(T*)((uint8_t*)(buckets) + index * bucketSize);
	}

	bool init(Heap* heap, size_t bucketSize, size_t bucketsNum);
	void release();

	void* allocate();
	void deallocate(void* bucket);

	void clear();

	Heap* getHeap() { return heap; }

	size_t size() const { return usingBucketsNum; }

	size_t getBucketSize() const { return bucketSize; }
	size_t getBucketsNum() const { return bucketsNum; }

	bool isFull() const { return usingBucketsNum == bucketsNum; }
	bool isEmpty() const { return usingBucketsNum == 0; }
	bool isInit() const { return buckets; }
};