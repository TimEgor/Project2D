#pragma once

class ChunkAllocator final {
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
	~ChunkAllocator() { ChunkAllocator::release(); }

	ChunkAllocator& operator=(const ChunkAllocator&) = delete;
	ChunkAllocator& operator=(ChunkAllocator&& allocator);

	bool init(size_t chunkSize, size_t chunksNum);
	void release();

	void* allocate(size_t size);
	void deallocate(void* sector);

	void clean();

	size_t size() const { return usingChunksNum; }
	size_t getUsingSectorsNum() const { return usingSectorsNum; }

	size_t getChunkSize() const { return chunkSize; }
	size_t getChunkNum() const { return chunksNum; }

	bool isFull() const { return usingChunksNum == chunksNum; }
	bool isEmpty() const { return usingChunksNum == 0; }
	bool isInit() const { return chunks; }
};