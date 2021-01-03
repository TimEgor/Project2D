#pragma once

#include <MemoryManager/Heap.h>


constexpr size_t bigAllocationAligment = 32;
constexpr size_t bigAllocationAdditionalData = sizeof(void*) + bigAllocationAligment - 1;
constexpr size_t bigAllocationThreshold = 4096;

template<typename T>
class CustomHeapAllocator final {
    template <typename>
    friend class CustomHeapAllocator;

private:
    Heap* heap;

    inline void* allocateBigData(size_t size) {
        const size_t requiringSize = size + bigAllocationAdditionalData;
        const size_t blockPtr = (size_t)(heap->allocate(requiringSize));
        void* const ptr = (void*)((blockPtr + bigAllocationAdditionalData) & ~(bigAllocationAligment - 1));
        ((size_t*)(ptr))[-1] = blockPtr;

        return ptr;
    }

    inline void* getBigData(void* ptr, size_t size) {
        size_t *blockPtr = &(((size_t*)(ptr))[-1]);
        return (void*)(*blockPtr);
    }

    inline void* heapAllocate(size_t size) {
        if (size >= bigAllocationThreshold) {
            return allocateBigData(size);
        }

        if (size != 0) {
            return heap->allocate(size);
        }

        return nullptr;
    }

    inline void heapDeallocate(void* ptr, size_t size) {
        if (size >= bigAllocationThreshold) {
            ptr = getBigData(ptr, size);
        }

        heap->deallocate(ptr);
    }

public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    CustomHeapAllocator() = delete;
    explicit CustomHeapAllocator(Heap *heap) : heap(heap) {}
    CustomHeapAllocator(const CustomHeapAllocator&) = default;
    CustomHeapAllocator& operator=(const CustomHeapAllocator&) = default;

    template<class U>
    CustomHeapAllocator(const CustomHeapAllocator<U>& allocator) : heap(allocator.heap) {}

    T* allocate(size_t n) {
        return (T*)(heapAllocate(sizeof(T) * n));
    }

    void deallocate(T* ptr, size_t n) {
        heapDeallocate(ptr, sizeof(T) * n);
    }

    template< class U, class... Args >
    void construct(U* ptr, Args&&... args) {
        new(ptr) U((Args&&)(args)...);
    }

    template< class U >
    void destroy(U* ptr) {
        ptr->~U();
    }
};