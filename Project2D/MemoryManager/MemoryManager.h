#pragma once

#include <MemoryManager/Heap.h>

class MemoryManager final {
private:
	MemoryManager();

public:
	static MemoryManager& get();

	static inline Heap* getDefaultHeap() {
		static Heap defaultHeap = GetProcessHeap();
		return &defaultHeap;
	}
};