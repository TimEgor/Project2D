#include "Heap.h"

#include <cassert>

Heap::Heap(Heap&& heap) {
    handle = heap.handle;
    heap.handle = NULL;
}

Heap& Heap::operator=(Heap&& heap) {
    if (handle)
        release();

    handle = heap.handle;
    heap.handle = NULL;

    return *this;
}

bool Heap::init() {
    assert(!handle);
    handle = HeapCreate(NULL, 0, 0);
    assert(handle);

    return handle;
}

void Heap::release() {
    assert(handle);
    bool destroyingResult = HeapDestroy(handle);
    assert(destroyingResult);
}

void* Heap::allocate(size_t elementNum) {
    //assert(handle);
    void* m = HeapAlloc(handle, NULL, elementNum);
    return m;
}

void* Heap::reallocate(void* mem, size_t elementNum) {
    assert(handle);
    return HeapReAlloc(handle, NULL, mem, elementNum);
}

void Heap::deallocate(void* mem) {
    //assert(handle);
    bool deallocatingResult = HeapFree(handle, NULL, mem);
    assert(deallocatingResult);
}

size_t Heap::getMemSize(void* mem) {
    assert(handle);
    return HeapSize(handle, NULL, mem);
}