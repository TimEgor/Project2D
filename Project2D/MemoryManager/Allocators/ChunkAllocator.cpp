#include "ChunkAllocator.h"

#include <MemoryManager/Heap.h>
#include <MemoryManager/Allocators/AllocatorsUtilities.h>

#include <cassert>
#include <memory>
#include <cmath>
#include <cstdint>

ChunkAllocator::ChunkAllocator()
    : heap(nullptr), chunks(nullptr), firstFreeSector(nullptr),
    usingChunksNum(0), usingSectorsNum(0), chunkSize(0), chunksNum(0) {
}

ChunkAllocator::ChunkAllocator(ChunkAllocator&& allocator) {
    heap = allocator.heap;
    chunks = allocator.chunks;
    firstFreeSector = allocator.firstFreeSector;
    usingChunksNum = allocator.usingChunksNum;
    usingSectorsNum = allocator.usingSectorsNum;
    chunkSize = allocator.chunkSize;
    chunksNum = allocator.chunksNum;

    allocator.heap = nullptr;
    allocator.chunks = nullptr;
    allocator.firstFreeSector = nullptr;
    allocator.usingChunksNum = 0;
    allocator.chunkSize = 0;
    allocator.chunksNum = 0;
}

ChunkAllocator& ChunkAllocator::operator=(ChunkAllocator&& allocator) {
    release();

    heap = allocator.heap;
    chunks = allocator.chunks;
    firstFreeSector = allocator.firstFreeSector;
    usingChunksNum = allocator.usingChunksNum;
    usingSectorsNum = allocator.usingSectorsNum;
    chunkSize = allocator.chunkSize;
    chunksNum = allocator.chunksNum;

    allocator.heap = nullptr;
    allocator.chunks = nullptr;
    allocator.firstFreeSector = nullptr;
    allocator.usingChunksNum = 0;
    allocator.chunkSize = 0;
    allocator.chunksNum = 0;

    return *this;
}

bool ChunkAllocator::init(Heap* _heap, size_t _chunkSize, size_t _chunksNum) {
    size_t requiredSize = (_chunkSize + sizeof(SectorInfo)) * _chunksNum;
    chunks = _heap->allocate(requiredSize);
    if (!chunks) {
        return false;
    }

    heap = _heap;

    chunkSize = _chunkSize;
    chunksNum = _chunksNum;

    return true;
}

void ChunkAllocator::release() {
    if (chunks) {
        heap->deallocate(chunks);
        chunks = nullptr;
    }

    heap = nullptr;
    firstFreeSector = nullptr;

    usingChunksNum = 0;
    usingSectorsNum = 0;

    chunkSize = 0;
    chunksNum = 0;
}

void* ChunkAllocator::allocate(size_t size) {
    void* requiringSector = nullptr;

    size_t requiringSize = size + sizeof(SectorInfo);
    size_t requiringChunks = (size_t)ceilf(requiringSize / (float)chunkSize);

    if (requiringChunks < chunksNum - usingChunksNum) {
        SectorInfo* currentSectorInfo = firstFreeSector;

        while (currentSectorInfo->chunksNum < requiringChunks) {
            if (currentSectorInfo->nextFreeSector == nullptr)
                return requiringSector;
            else
                currentSectorInfo = currentSectorInfo->nextFreeSector;
        }

        SectorInfo* newSectorInfo = (SectorInfo*)(((uint8_t*)currentSectorInfo + requiringSize));

        newSectorInfo->isFree = true;
        newSectorInfo->nextSector = currentSectorInfo->nextSector;
        newSectorInfo->prevSector = currentSectorInfo;
        newSectorInfo->nextFreeSector = currentSectorInfo->nextFreeSector;
        newSectorInfo->prevFreeSector = currentSectorInfo->prevFreeSector;
        newSectorInfo->chunksNum = currentSectorInfo->chunksNum - requiringChunks;

        currentSectorInfo->nextSector = newSectorInfo;
        currentSectorInfo->chunksNum = requiringChunks;

        firstFreeSector = newSectorInfo;

        if (currentSectorInfo->prevFreeSector)
            currentSectorInfo->prevFreeSector->nextFreeSector = currentSectorInfo->nextFreeSector;

        currentSectorInfo->isFree = false;
        requiringSector = (uint8_t*)currentSectorInfo + sizeof(SectorInfo);

        usingChunksNum += requiringChunks;
        ++usingSectorsNum;
    }

    return requiringSector;
}

void ChunkAllocator::deallocate(void* sector) {
    SectorInfo* currentSectionInfo = (SectorInfo*)((uint8_t*)sector - sizeof(SectorInfo));

    currentSectionInfo->isFree = true;
    usingChunksNum -= currentSectionInfo->chunksNum;

    SectorInfo* nearSector = currentSectionInfo->prevSector;
    if (nearSector && nearSector->isFree) {
        nearSector->nextSector = currentSectionInfo->nextSector;
        nearSector->chunksNum += currentSectionInfo->chunksNum;

        currentSectionInfo = nearSector;
    }

    nearSector = currentSectionInfo->nextSector;
    if (nearSector && nearSector->isFree) {
        currentSectionInfo->nextSector = nearSector->nextSector;
        currentSectionInfo->nextFreeSector = nearSector->nextFreeSector;
        currentSectionInfo->chunksNum += nearSector->chunksNum;

        if (nearSector == firstFreeSector)
            firstFreeSector = currentSectionInfo;
    }

    --usingSectorsNum;
}

void ChunkAllocator::clean() {
    firstFreeSector = (SectorInfo*)chunks;
    firstFreeSector->isFree = true;
    firstFreeSector->nextSector = nullptr;
    firstFreeSector->prevSector = nullptr;
    firstFreeSector->nextFreeSector = nullptr;
    firstFreeSector->prevFreeSector = nullptr;
    firstFreeSector->chunksNum = chunksNum;

    usingChunksNum = 0;
    usingSectorsNum = 0;
}
