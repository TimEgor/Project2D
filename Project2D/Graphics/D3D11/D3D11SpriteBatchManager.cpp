#include "D3D11SpriteBatchManager.h"

#include <Graphics/RenderingOrder.h>

#include <cassert>

D3D11SpriteBatchManager::D3D11SpriteBatchManager() : preparingBatchesCount(0) {}

void D3D11SpriteBatchManager::prepareRenderingData(const std::vector<RenderingOrderNode*>& nodes, RenderingOrderType  nodesType) {
	if (nodes.empty()) {
		return;
	}

	size_t nodesCount = nodes.size();

	size_t currentBatchesCount = batches.size();
	size_t currentCapacity = currentBatchesCount * BATCH_SIZE;

	if (currentCapacity < nodesCount) {
		size_t deltaCount = nodesCount - currentCapacity;
		size_t requiringNewBatchesCount = deltaCount / BATCH_SIZE;
		if (deltaCount % BATCH_SIZE != 0) {
			++requiringNewBatchesCount;
		}

		batches.resize(currentBatchesCount + requiringNewBatchesCount);
	}

	size_t requiringBatchesCount = nodesCount / BATCH_SIZE;
	if (nodesCount % BATCH_SIZE != 0) {
		++requiringBatchesCount;
	}

	size_t currentIndex = 0;
	size_t currentNodesCount = nodesCount;

	for (size_t i = 0; i < requiringBatchesCount; ++i) {
		D3D11SpriteBatch& batch = batches[i];
		
		if (!batch.isInit()) {
			batch.init(BATCH_SIZE);
		}

		size_t currentBatchNodesCount = currentNodesCount < BATCH_SIZE ? currentNodesCount : BATCH_SIZE;
		batch.buildData(nodes, currentIndex, currentBatchNodesCount, nodesType);
		
		currentIndex += currentBatchNodesCount;
		currentNodesCount -= currentBatchNodesCount;
	}

	preparingBatchesCount = requiringBatchesCount;
}

void D3D11SpriteBatchManager::releaseUselessBranches() {
	size_t currentBatchesCount = batches.size();
	size_t uselessBatchesCount = currentBatchesCount - preparingBatchesCount;

	for (size_t i = 0; i < uselessBatchesCount; ++i) {
		D3D11SpriteBatch& uselessBatch = batches[currentBatchesCount - i - 1];

		if (uselessBatch.isInit()) {
			uselessBatch.release();
		}

		batches.pop_back();
	}
}

void D3D11SpriteBatchManager::release() {
	batches = std::vector<D3D11SpriteBatch>();
	preparingBatchesCount = 0;
}
