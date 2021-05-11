#pragma once

#include <Graphics/D3D11/D3D11SpriteBatch.h>

#include <vector>

#define BATCH_SIZE 2048

class D3D11SpriteBatchManager final {
private:
	std::vector<D3D11SpriteBatch> batches;
	size_t preparingBatchesCount;

public:
	D3D11SpriteBatchManager();

	void prepareRenderingData(const std::vector<RenderingOrderNode*>& nodes, RenderingOrderType nodesType);

	void releaseUselessBranches();
	void release();

	const size_t getPreparingBatchesCount() const { return preparingBatchesCount; }
	const std::vector<D3D11SpriteBatch>& getBatches() const { return batches; }
};