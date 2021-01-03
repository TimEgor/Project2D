#include "MemoryManager.h"

#include <Windows.h>

MemoryManager::MemoryManager() : defaultHeap(GetProcessHeap()) {
}

MemoryManager& MemoryManager::get() {
	static MemoryManager uniqueMemoryManager;
	return uniqueMemoryManager;
}
