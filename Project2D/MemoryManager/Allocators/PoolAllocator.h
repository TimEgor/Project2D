#pragma once

class Heap;

class PoolAllocator {
protected:
	struct FreeBucketInfo final {
		FreeBucketInfo* nextFree;
	};

protected:
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
	virtual ~PoolAllocator() { PoolAllocator::release(); }

	PoolAllocator& operator=(const PoolAllocator&) = delete;
	PoolAllocator& operator=(PoolAllocator&& allocator);

	virtual bool init(Heap *heap, size_t bucketSize, size_t bucketsNum);
	virtual void release();

	virtual void* allocate();
	virtual void deallocate(void* bucket);

	virtual void clear();

	virtual Heap* getHeap() { return heap; }

	virtual size_t size() const { return usingBucketsNum; }

	virtual size_t getBucketSize() const { return bucketSize; }
	virtual size_t getBucketsNum() const { return bucketsNum; }

	virtual bool isFull() const { return usingBucketsNum == bucketsNum; }
	virtual bool isInit() const { return buckets; }
};