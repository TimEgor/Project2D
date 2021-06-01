#pragma once

#include <Windows.h>

#include <cassert>

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

	inline void* allocate(size_t elementNum) {
		//assert(handle);
		void* m = HeapAlloc(handle, NULL, elementNum);
		return m;
	}

	inline void* reallocate(void* mem, size_t elementNum) {
		assert(handle);
		return HeapReAlloc(handle, NULL, mem, elementNum);
	}

	inline void deallocate(void* mem) {
		//assert(handle);
		bool deallocatingResult = HeapFree(handle, NULL, mem);
		assert(deallocatingResult);
	}

	size_t getMemSize(void* mem);
};