#pragma once

#include <MemoryManager/Allocators/PoolAllocator.h>

class Heap;

class LinkingPoolAllocator : public PoolAllocator {
protected:
	struct UsingBucketInfo final {
		UsingBucketInfo* nextBucket;
		UsingBucketInfo* prevBucket;
	};

	UsingBucketInfo* lastBucketInfo;

	UsingBucketInfo* firstUsingBucketInfo;

public:
	LinkingPoolAllocator() : lastBucketInfo(nullptr), firstUsingBucketInfo(nullptr) {}
	virtual ~LinkingPoolAllocator() { LinkingPoolAllocator::release(); }

	virtual bool init(Heap* heap, size_t bucketSize, size_t bucketsNum) override;

	virtual void clear() override;

	virtual void* allocate() override;
	virtual void deallocate(void* bucket) override;

	const UsingBucketInfo* getFirstUsingBucketInfo() { return firstUsingBucketInfo; }
};