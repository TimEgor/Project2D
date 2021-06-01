#pragma once

class LinkingPoolAllocator {
private:
	struct FreeBucketInfo final {
		FreeBucketInfo* nextFree;
	};

	struct UsingBucketInfo final {
		UsingBucketInfo* nextBucket;
		UsingBucketInfo* prevBucket;
	};

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

	bool init(size_t bucketSize, size_t bucketsNum);
	void release();

	void* allocate();
	void deallocate(void* bucket);

	void clear();

	size_t size() const { return usingBucketsNum; }

	size_t getBucketSize() const { return bucketSize; }
	size_t getBucketsNum() const { return bucketsNum; }

	bool isFull() const { return usingBucketsNum == bucketsNum; }
	bool isEmpty() const { return usingBucketsNum == 0; }
	bool isInit() const { return buckets; }

	const UsingBucketInfo* getFirstUsingBucketInfo() { return firstUsingBucketInfo; }
};