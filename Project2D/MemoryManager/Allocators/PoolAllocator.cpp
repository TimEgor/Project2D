#include "PoolAllocator.h"

#include <MemoryManager/MemoryCore.h>
#include <MemoryManager/Allocators/AllocatorsUtilities.h>

#include <cassert>
#include <memory>

PoolAllocator::PoolAllocator()
    : buckets(nullptr), firstFreeBucket(nullptr),
    usingBucketsNum(0), bucketSize(0), bucketsNum(0) {}

PoolAllocator::PoolAllocator(PoolAllocator&& allocator) {
    buckets = allocator.buckets;
    firstFreeBucket = allocator.firstFreeBucket;
    usingBucketsNum = allocator.usingBucketsNum;
    bucketSize = allocator.bucketSize;
    bucketsNum = allocator.bucketsNum;

    allocator.buckets = nullptr;
    allocator.firstFreeBucket = nullptr;
    allocator.usingBucketsNum = 0;
    allocator.bucketSize = 0;
    allocator.bucketsNum = 0;
}

PoolAllocator& PoolAllocator::operator=(PoolAllocator&& allocator) {
    release();

    buckets = allocator.buckets;
    firstFreeBucket = allocator.firstFreeBucket;
    usingBucketsNum = allocator.usingBucketsNum;
    bucketSize = allocator.bucketSize;
    bucketsNum = allocator.bucketsNum;

    allocator.buckets = nullptr;
    allocator.firstFreeBucket = nullptr;
    allocator.usingBucketsNum = 0;
    allocator.bucketSize = 0;
    allocator.bucketsNum = 0;

    return *this;
}

bool PoolAllocator::init(size_t _bucketSize, size_t _bucketsNum) {
    assert(_bucketSize > sizeof(FreeBucketInfo));

    size_t requiredSize = _bucketSize * _bucketsNum;
    buckets = new uint8_t [requiredSize];
    if (!buckets) {
        return false;
    }

    bucketSize = _bucketSize;
    bucketsNum = _bucketsNum;

    clear();

    return true;
}

void PoolAllocator::release() {
    if (buckets) {
        delete[] buckets;
        buckets = nullptr;
    }

    firstFreeBucket = nullptr;

    usingBucketsNum = 0;
    bucketSize = 0;
    bucketsNum = 0;
}

void* PoolAllocator::allocate() {
    void* requiringBucket = nullptr;

    if (firstFreeBucket) {
        requiringBucket = firstFreeBucket;
        firstFreeBucket = firstFreeBucket->nextFree;

        ++usingBucketsNum;
    }

    return requiringBucket;
}

void PoolAllocator::deallocate(void* bucket) {
    FreeBucketInfo* nextFreeBucket = (FreeBucketInfo*)bucket;
    nextFreeBucket->nextFree = firstFreeBucket;

    firstFreeBucket = nextFreeBucket;

    --usingBucketsNum;
}

void PoolAllocator::clear() {
    firstFreeBucket = (FreeBucketInfo*)buckets;

    FreeBucketInfo* iterSectorInfo = firstFreeBucket;
    for (size_t i = 0; i < bucketsNum; ++i) {
        iterSectorInfo->nextFree = (FreeBucketInfo*)((uint8_t*)firstFreeBucket + (bucketSize * i));
        iterSectorInfo = iterSectorInfo->nextFree;
    }
    iterSectorInfo->nextFree = nullptr;

    usingBucketsNum = 0;
}
