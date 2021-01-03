#pragma once

#include <Windows.h>

class HeapReference;

class Heap final {
	friend class HeapReference;

private:
	HANDLE handle = NULL;

public:
	Heap() = default;
	Heap(HANDLE heapHandle) : handle(heapHandle) {}
	Heap(Heap&& heap);

	Heap& operator=(Heap&& heap);

	bool init();
	void release();

	void* allocate(size_t elementNum);
	void* reallocate(void* mem, size_t elementNum);
	void deallocate(void* mem);

	size_t getMemSize(void* mem);
};