#pragma once

#include <MemoryManager/CustomHeapAllocator.h>

#include <vector>
#include <unordered_map>

#define CustonHeapVector(Type) std::vector<type, CustomHeapAllocator<Type>>
#define CustomHeapVectorAllocator(Type) CustomHeapAllocator<Type>

#define CustomHeapUnorderedMap(KeyType, ValType) std::unordered_map<KeyType, ValType, std::hash<KeyType>, std::equal_to<KeyType>, CustomHeapAllocator<std::pair<const KeyType, ValType>>>
#define CustomHeapUnorderedMapAllocator(KeyType, ValType) CustomHeapAllocator<std::pair<const KeyType, ValType>>