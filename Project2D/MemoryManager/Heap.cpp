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

inline void Heap::release() {
    assert(handle);
    bool destroyingResult = HeapDestroy(handle);
    assert(destroyingResult);
}

size_t Heap::getMemSize(void* mem) {
    assert(handle);
    return HeapSize(handle, NULL, mem);
}