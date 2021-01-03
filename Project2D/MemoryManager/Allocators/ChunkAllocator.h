#pragma once

class Heap;

class ChunkAllocator {
public:
	struct SectorInfo final {
		size_t chunksNum;
		SectorInfo* nextSector;
		SectorInfo* prevSector;
		SectorInfo* nextFreeSector;
		SectorInfo* prevFreeSector;
		bool isFree;
	};

protected:
	Heap* heap;

	void* chunks;

	SectorInfo* firstFreeSector;
	size_t usingChunksNum;
	size_t usingSectorsNum;

	size_t chunkSize;
	size_t chunksNum;

public:
	ChunkAllocator();
	ChunkAllocator(const ChunkAllocator&) = delete;
	ChunkAllocator(ChunkAllocator&& allocator);
	virtual ~ChunkAllocator() { ChunkAllocator::release(); }

	ChunkAllocator& operator=(const ChunkAllocator&) = delete;
	ChunkAllocator& operator=(ChunkAllocator&& allocator);

	virtual bool init(Heap* heap, size_t chunkSize, size_t chunksNum);
	virtual void release();

	virtual void* allocate(size_t size);
	virtual void deallocate(void* sector);

	virtual void clean();

	virtual Heap* getHeap() { return heap; }

	virtual size_t size() const { return usingChunksNum; }
	virtual size_t getUsingSectorsNum() const { return usingSectorsNum; }

	virtual size_t getChunkSize() const { return chunkSize; }
	virtual size_t getChunkNum() const { return chunksNum; }

	virtual bool isFull() const { return usingChunksNum == chunksNum; }
	virtual bool isInit() const { return chunks; }
};