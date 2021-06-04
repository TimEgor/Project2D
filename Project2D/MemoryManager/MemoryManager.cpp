#include "MemoryManager.h"

#include <Windows.h>

MemoryManager::MemoryManager() {
}

MemoryManager& MemoryManager::get() {
	static MemoryManager uniqueMemoryManager;
	return uniqueMemoryManager;
}
