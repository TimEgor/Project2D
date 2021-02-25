#include "LinkingPoolAllocator.h"

#include <MemoryManager/Heap.h>
#include <MemoryManager/Allocators/AllocatorsUtilities.h>

#include <cassert>
#include <memory>
#include <cstdint>

bool LinkingPoolAllocator::init(Heap* _heap, size_t _bucketSize, size_t _bucketsNum) {
    size_t requiredBucketSize = _bucketSize + sizeof(UsingBucketInfo);

    assert(requiredBucketSize > sizeof(FreeBucketInfo));

    size_t requiredSize = requiredBucketSize * _bucketsNum;
    buckets = _heap->allocate(requiredSize);
    if (!buckets) {
        return false;
    }

    heap = _heap;

    bucketSize = _bucketSize;
    bucketsNum = _bucketsNum;

    lastBucketInfo = (UsingBucketInfo*)((uint8_t*)buckets + bucketsNum * (bucketSize + sizeof(UsingBucketInfo)) - sizeof(UsingBucketInfo));

    clear();

    return true;
}

void LinkingPoolAllocator::clear() {
    firstUsingBucketInfo = nullptr;

    firstFreeBucket = (FreeBucketInfo*)buckets;

    FreeBucketInfo* iterSectorInfo = firstFreeBucket;
    for (size_t i = 0; i < bucketsNum; ++i) {
        iterSectorInfo->nextFree = (FreeBucketInfo*)((uint8_t*)firstFreeBucket + (bucketSize * i));
        iterSectorInfo = iterSectorInfo->nextFree;
    }
    iterSectorInfo->nextFree = nullptr;

    usingBucketsNum = 0;
}

void LinkingPoolAllocator::release() {
    if (buckets) {
        heap->deallocate(buckets);
        buckets = nullptr;
    }

    heap = nullptr;
    firstFreeBucket = nullptr;

    usingBucketsNum = 0;
    bucketSize = 0;
    bucketsNum = 0;
}

void* LinkingPoolAllocator::allocate() {
    void* requiringBucket = nullptr;

    if (firstFreeBucket) {
        requiringBucket = firstFreeBucket;
        firstFreeBucket = firstFreeBucket->nextFree;

        ++usingBucketsNum;

        UsingBucketInfo* currentBuckerInfo = (UsingBucketInfo*)((uint8_t*)requiringBucket + bucketSize);
        currentBuckerInfo->nextBucket = nullptr;
        currentBuckerInfo->prevBucket = nullptr;

        if (requiringBucket != buckets) {
            UsingBucketInfo* prevBucketInfo = (UsingBucketInfo*)((uint8_t*)requiringBucket - sizeof(UsingBucketInfo*));
            currentBuckerInfo->prevBucket = prevBucketInfo;
            prevBucketInfo->nextBucket = currentBuckerInfo;
        }

        if (currentBuckerInfo != lastBucketInfo) {
            UsingBucketInfo* nextBucketInfo = (UsingBucketInfo*)((uint8_t*)currentBuckerInfo + sizeof(UsingBucketInfo*) + bucketSize);
            currentBuckerInfo->nextBucket = nextBucketInfo;
            nextBucketInfo->prevBucket = currentBuckerInfo;
        }

        if (firstUsingBucketInfo) {
            if (currentBuckerInfo < firstUsingBucketInfo) {
                firstUsingBucketInfo = currentBuckerInfo;
            }
        }
        else {
            firstUsingBucketInfo = currentBuckerInfo;
        }
    }

    return requiringBucket;
}

void LinkingPoolAllocator::deallocate(void* bucket) {
    UsingBucketInfo* currentBuckerInfo = (UsingBucketInfo*)((uint8_t*)bucket + bucketSize);
    if (bucket != buckets) {
        UsingBucketInfo* prevBucketInfo = (UsingBucketInfo*)((uint8_t*)bucket - sizeof(UsingBucketInfo*));
        prevBucketInfo->nextBucket = currentBuckerInfo->nextBucket;
    }

    if (currentBuckerInfo != lastBucketInfo) {
        UsingBucketInfo* nextBucketInfo = (UsingBucketInfo*)((uint8_t*)currentBuckerInfo + sizeof(UsingBucketInfo*) + bucketSize);
        nextBucketInfo->prevBucket = currentBuckerInfo->prevBucket;
    }

    if (currentBuckerInfo == firstUsingBucketInfo) {
        firstUsingBucketInfo = firstUsingBucketInfo->nextBucket;
    }

    FreeBucketInfo* nextFreeBucket = (FreeBucketInfo*)bucket;
    nextFreeBucket->nextFree = firstFreeBucket;

    firstFreeBucket = nextFreeBucket;

    --usingBucketsNum;
}
