#pragma once

class Heap;

class PoolAllocator final {
	struct FreeBucketInfo final {
		FreeBucketInfo* nextFree;
	};

private:
	Heap* heap;

	void* buckets;

	FreeBucketInfo* firstFreeBucket;
	size_t usingBucketsNum;

	size_t bucketSize;
	size_t bucketsNum;

public:
	PoolAllocator();
	PoolAllocator(const PoolAllocator&) = delete;
	PoolAllocator(PoolAllocator&& allocator);
	~PoolAllocator() { release(); }

	PoolAllocator& operator=(const PoolAllocator&) = delete;
	PoolAllocator& operator=(PoolAllocator&& allocator);

	bool init(Heap *heap, size_t bucketSize, size_t bucketsNum);
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