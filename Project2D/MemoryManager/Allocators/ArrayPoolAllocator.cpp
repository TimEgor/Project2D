#include "ArrayPoolAllocator.h"

#include <MemoryManager/Heap.h>
#include <MemoryManager/Allocators/AllocatorsUtilities.h>

#include <cassert>
#include <memory>

ArrayPoolAllocator::ArrayPoolAllocator()
    : heap(nullptr), buckets(nullptr),
    usingBucketsNum(0), bucketSize(0), bucketsNum(0) {
}

ArrayPoolAllocator::ArrayPoolAllocator(ArrayPoolAllocator&& allocator) {
    heap = allocator.heap;
    buckets = allocator.buckets;
    usingBucketsNum = allocator.usingBucketsNum;
    bucketSize = allocator.bucketSize;
    bucketsNum = allocator.bucketsNum;

    allocator.heap = nullptr;
    allocator.buckets = nullptr;
    allocator.usingBucketsNum = 0;
    allocator.bucketSize = 0;
    allocator.bucketsNum = 0;
}

ArrayPoolAllocator& ArrayPoolAllocator::operator=(ArrayPoolAllocator&& allocator) {
    release();

    heap = allocator.heap;
    buckets = allocator.buckets;
    usingBucketsNum = allocator.usingBucketsNum;
    bucketSize = allocator.bucketSize;
    bucketsNum = allocator.bucketsNum;

    allocator.heap = nullptr;
    allocator.buckets = nullptr;
    allocator.usingBucketsNum = 0;
    allocator.bucketSize = 0;
    allocator.bucketsNum = 0;

    return *this;
}

void* ArrayPoolAllocator::operator[](size_t index) {
    return (uint8_t*)(buckets) + index * bucketSize;
}

bool ArrayPoolAllocator::init(Heap* _heap, size_t _bucketSize, size_t _bucketsNum) {
    size_t requiredSize = _bucketSize * _bucketsNum;
    buckets = _heap->allocate(requiredSize);
    if (!buckets) {
        return false;
    }

    heap = _heap;

    bucketSize = _bucketSize;
    bucketsNum = _bucketsNum;

    clear();

    return true;
}

void ArrayPoolAllocator::release() {
    if (buckets) {
        heap->deallocate(buckets);
        buckets = nullptr;
    }

    heap = nullptr;

    usingBucketsNum = 0;
    bucketSize = 0;
    bucketsNum = 0;
}

void* ArrayPoolAllocator::allocate() {
    void* requiringBucket = nullptr;

    if (usingBucketsNum != bucketsNum) {
        requiringBucket = (uint8_t*)(buckets) + bucketSize * usingBucketsNum;
        ++usingBucketsNum;
    }

    return requiringBucket;
}

void ArrayPoolAllocator::deallocate(void* bucket) {
    if (usingBucketsNum != 1) {
        void* endBucket = (uint8_t*)(bucket) + ((usingBucketsNum - 1) * bucketSize);
        memcpy_s(bucket, bucketSize, endBucket, bucketSize);
    }

    --usingBucketsNum;
}

void ArrayPoolAllocator::clear() {
    usingBucketsNum = 0;
}
