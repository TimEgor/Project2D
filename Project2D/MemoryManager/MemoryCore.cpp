#include "MemoryCore.h"

#include <MemoryManager/MemoryManager.h>
#include <MemoryManager/Heap.h>

extern inline void* memAllocate(size_t size) {
    return memHeapAllocate(MemoryManager::getDefaultHeap(), size);
}

extern inline void* memReallocate(void* ptr, size_t size) {
    return memHeapReallocate(MemoryManager::getDefaultHeap(), ptr, size);
}

extern inline void memRelease(void* ptr) {
    memHeapRelease(MemoryManager::getDefaultHeap(), ptr);
}

extern inline void* memHeapAllocate(Heap* heap, size_t size) {
    return heap->allocate(size);
}

extern inline void* memHeapReallocate(Heap* heap, void* ptr, size_t size) {
    return heap->reallocate(ptr, size);
}

extern inline void memHeapRelease(Heap* heap, void* ptr) {
    heap->deallocate(ptr);
}
