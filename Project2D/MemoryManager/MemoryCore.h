#pragma once

class Heap;

void* memAllocate(size_t size);
void* memReallocate(void* ptr, size_t size);
void memRelease(void* ptr);

void* memHeapAllocate(Heap* heap, size_t size);
void* memHeapReallocate(Heap* heap, void* ptr,size_t size);
void memHeapRelease(Heap* heap, void* ptr);

