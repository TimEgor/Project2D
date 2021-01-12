#pragma once

class Heap;

class LinkingPoolAllocator {
private:
	struct FreeBucketInfo final {
		FreeBucketInfo* nextFree;
	};

	struct UsingBucketInfo final {
		UsingBucketInfo* nextBucket;
		UsingBucketInfo* prevBucket;
	};

	Heap* heap;

	void* buckets;

	FreeBucketInfo* firstFreeBucket;
	size_t usingBucketsNum;

	size_t bucketSize;
	size_t bucketsNum;

	UsingBucketInfo* lastBucketInfo;
	UsingBucketInfo* firstUsingBucketInfo;

public:
	LinkingPoolAllocator() : lastBucketInfo(nullptr), firstUsingBucketInfo(nullptr) {}
	~LinkingPoolAllocator() { release(); }

	bool init(Heap* heap, size_t bucketSize, size_t bucketsNum);

	void clear();
	void release();

	void* allocate();
	void deallocate(void* bucket);

	void clear();

	Heap* getHeap() { return heap; }

	size_t size() const { return usingBucketsNum; }

	size_t getBucketSize() const { return bucketSize; }
	size_t getBucketsNum() const { return bucketsNum; }

	bool isFull() const { return usingBucketsNum == bucketsNum; }
	bool isInit() const { return buckets; }

	const UsingBucketInfo* getFirstUsingBucketInfo() { return firstUsingBucketInfo; }
};