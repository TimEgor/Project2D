#pragma once

#include <MemoryManager/Heap.h>

class MemoryManager final {
private:
	Heap defaultHeap;

	MemoryManager();

public:
	static MemoryManager& get();

	Heap* getDefaultHeap() { return &defaultHeap; }
};